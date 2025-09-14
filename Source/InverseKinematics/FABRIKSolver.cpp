// MIT License

// Copyright (c) 2025 Mateusz Gancarz

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "InverseKinematics/FABRIKSolver.h"

#include <iostream>
#include <ranges>

namespace chs::ik
{
    FABRIKSolver::FABRIKSolver(int num_of_iterations)
        : num_of_iterations{num_of_iterations} {}

    void FABRIKSolver::solve(Chain& chain, const Effector& effector) const
    {
        if (chain.empty())
        {
            return;
        }
        
        if (effectorFartherThanChainLength(chain, effector))
        {
            performChainStraightening(chain, effector);
            return;
        }

        performFABRIKIterations(chain, effector);
    }

    bool FABRIKSolver::effectorFartherThanChainLength(const Chain& chain, const Effector& effector) const
    {
        const Segment& first_segment = chain.first();
        const float distance_from_origin_to_effector = glm::distance(first_segment.worldOrigin(), effector.world_location);
        const float chain_total_length = chain.calculateTotalLength();
        return chain_total_length <= distance_from_origin_to_effector;
    }

    void FABRIKSolver::performChainStraightening(Chain& chain, const Effector& effector) const
    {
        const Segment& first_segment = chain.first();
        const glm::vec3 to_effector = glm::normalize(effector.world_location - first_segment.worldOrigin());

        glm::vec3 previous_segment_world_end = first_segment.worldOrigin();
        for (auto& segment : chain.segments())
        {
            const float segment_length = segment.length();
            segment.setWorldOrigin(previous_segment_world_end);

            const glm::vec3 segment_new_world_end = segment.worldOrigin() + to_effector * segment_length;
            segment.setWorldEnd(segment_new_world_end);
            previous_segment_world_end = segment_new_world_end;
        }
    }

    void FABRIKSolver::performFABRIKIterations(Chain& chain, const Effector& effector) const
    {
        Effector chain_origin = createEffectorFromChainOrigin(chain);

        for (int iteration = 0; iteration < num_of_iterations; ++iteration)
        {
            performForwardReachingPass(chain, effector);
            performBackwardReachingPass(chain, chain_origin);
        }
    }

    Effector FABRIKSolver::createEffectorFromChainOrigin(const Chain& chain) const
    {
        Effector chain_origin_effector{};
        chain_origin_effector.world_location = chain.first().worldOrigin();

        return chain_origin_effector;
    }

    void FABRIKSolver::performForwardReachingPass(Chain& chain, const Effector& effector) const
    {
        glm::vec3 previous_segment_origin = effector.world_location;
        for (auto& segment : std::views::reverse(chain.segments()))
        {
            const float current_segment_length = segment.length();
            const glm::vec3 to_origin_direction = glm::normalize(segment.worldOrigin() - previous_segment_origin);
            const glm::vec3 current_segment_new_origin = previous_segment_origin + to_origin_direction * current_segment_length;
            segment.setWorldOrigin(current_segment_new_origin);
            segment.setWorldEnd(previous_segment_origin);
            previous_segment_origin = current_segment_new_origin;
        }
    }

    void FABRIKSolver::performBackwardReachingPass(Chain& chain, const Effector& effector) const
    {
        glm::vec3 previous_segment_end = effector.world_location;
        for (auto& segment : chain.segments())
        {
            const float current_segment_length = segment.length();
            const glm::vec3 to_end_direction = glm::normalize(segment.worldEnd() - previous_segment_end);
            const glm::vec3 current_segment_new_end = previous_segment_end + to_end_direction * current_segment_length;
            segment.setWorldOrigin(previous_segment_end);
            segment.setWorldEnd(current_segment_new_end);
            previous_segment_end = current_segment_new_end;
        }
    }
}
