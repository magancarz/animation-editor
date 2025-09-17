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

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Utils/LinearAlgebra.h"
#include "InverseKinematics/ChainIterator.h"
#include "InverseKinematics/ChainReverseIterator.h"

namespace chs::ik
{
    FABRIKSolver::FABRIKSolver(int num_of_iterations)
        : num_of_iterations{num_of_iterations} {}

    void FABRIKSolver::solve(Chain& chain, const Effector& end_effector) const
    {
        static constexpr std::vector<Effector> middle_effectors{};
        solve(chain, end_effector, middle_effectors);
    }

    void FABRIKSolver::solve(Chain& chain, const Effector& end_effector, const std::vector<Effector>& middle_effectors) const
    {
        if (chain.empty())
        {
            return;
        }
        
        if (effectorFartherThanChainLength(chain, end_effector))
        {
            performChainStraightening(chain, end_effector);
            return;
        }

        performFABRIKIterations(chain, end_effector, middle_effectors);
    }

    bool FABRIKSolver::effectorFartherThanChainLength(const Chain& chain, const Effector& end_effector) const
    {
        const Segment& first_segment = chain.first();
        const float distance_from_origin_to_effector = glm::distance(first_segment.worldOrigin(), end_effector.world_location);
        return chain.length() <= distance_from_origin_to_effector;
    }

    void FABRIKSolver::performChainStraightening(Chain& chain, const Effector& end_effector) const
    {
        const Segment& first_segment = chain.first();
        const glm::vec3 chain_world_origin = first_segment.worldOrigin();
        const glm::vec3 to_effector_direction = glm::normalize(end_effector.world_location - chain_world_origin);
        glm::mat4 segment_world_rotation = glm::toMat4(glm::quat{glm::vec3{0, 1, 0}, to_effector_direction});

        glm::vec3 previous_segment_world_end = chain_world_origin;
        ChainIterator chain_iterator{chain};
        while (chain_iterator.hasNext())
        {
            Segment& segment = chain_iterator.next();
            const float current_segment_length = segment.length();
            glm::vec3 current_segment_world_location =
                previous_segment_world_end + to_effector_direction * current_segment_length;
            glm::mat4 segment_world_translation = glm::translate(glm::mat4{1.0f}, current_segment_world_location);
            glm::mat4 segment_world_transform = segment_world_translation * segment_world_rotation;
            segment.setWorldTransform(segment_world_transform);
            previous_segment_world_end = current_segment_world_location;
        }
    }

    void FABRIKSolver::performFABRIKIterations(Chain& chain, const Effector& end_effector, const std::vector<Effector>& middle_effectors) const
    {
        const Effector chain_origin = createEffectorFromChainOrigin(chain);

        for (int iteration = 0; iteration < num_of_iterations; ++iteration)
        {
            performForwardReachingPass(chain, end_effector, middle_effectors);
            performBackwardReachingPass(chain, chain_origin);
        }

        Segment& chain_root = chain.first();
        chain_root.refresh();
    }

    Effector FABRIKSolver::createEffectorFromChainOrigin(const Chain& chain) const
    {
        Effector chain_origin_effector{};
        chain_origin_effector.world_location = chain.first().worldOrigin();

        return chain_origin_effector;
    }

    void FABRIKSolver::performForwardReachingPass(Chain& chain, const Effector& end_effector, const std::vector<Effector>& middle_effectors) const
    {
        glm::vec3 previous_segment_origin = end_effector.world_location;
        ChainReverseIterator chain_reverse_iterator{chain};
        while (chain_reverse_iterator.hasNext())
        {
            Segment& current_segment = chain_reverse_iterator.next();
            const float current_segment_length = current_segment.length();
            const glm::vec3 current_segment_world_origin = current_segment.worldOrigin();
            const glm::vec3 to_middle_effectors = calculateToMiddleEffectorsDirection(previous_segment_origin, middle_effectors);
            const glm::vec3 to_world_origin_direction = glm::safeNormalize(
                current_segment_world_origin - previous_segment_origin + to_middle_effectors);
            const glm::mat4 current_segment_world_translation = glm::translate(glm::mat4{1.0f}, previous_segment_origin);
            const glm::mat4 current_segment_world_rotation = glm::toMat4(glm::quat{glm::vec3{0, 1, 0}, -to_world_origin_direction});
            current_segment.forceWorldTransform(current_segment_world_translation * current_segment_world_rotation);
            previous_segment_origin = previous_segment_origin + to_world_origin_direction * current_segment_length;
        }
    }

    glm::vec3 FABRIKSolver::calculateToMiddleEffectorsDirection(const glm::vec3& from, const std::vector<Effector>& middle_effectors) const
    {
        if (middle_effectors.empty())
        {
            return glm::vec3{0};
        }

        glm::vec3 accumulated_directions{0.0f};
        for (const auto& middle_effector : middle_effectors)
        {
            accumulated_directions += middle_effector.world_location - from;
        }

        return glm::safeNormalize(accumulated_directions);
    }

    void FABRIKSolver::performBackwardReachingPass(Chain& chain, const Effector& end_effector) const
    {
        glm::vec3 previous_segment_end = end_effector.world_location;
        ChainIterator chain_iterator{chain};
        while (chain_iterator.hasNext())
        {
            Segment& current_segment = chain_iterator.next();
            const float current_segment_length = current_segment.length();
            const glm::vec3 to_world_end_direction = glm::safeNormalize(current_segment.worldEnd() - previous_segment_end);
            const glm::vec3 current_segment_world_end = previous_segment_end + to_world_end_direction * current_segment_length;
            const glm::mat4 current_segment_world_translation = glm::translate(glm::mat4{1.0f}, current_segment_world_end);
            const glm::mat4 current_segment_world_rotation = glm::toMat4(glm::quat{glm::vec3{0, 1, 0}, to_world_end_direction});
            current_segment.forceWorldTransform(current_segment_world_translation * current_segment_world_rotation);
            previous_segment_end = current_segment_world_end;
        }
    }
}
