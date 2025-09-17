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

#pragma once

#include "Common/Chain.h"
#include "Common/Effector.h"

namespace chs::ik
{
    class FABRIKSolver
    {
    public:
        explicit FABRIKSolver(int num_of_iterations = 1);

        FABRIKSolver(const FABRIKSolver&) = default;
        FABRIKSolver& operator=(const FABRIKSolver&) = default;
        FABRIKSolver(FABRIKSolver&&) noexcept = default;
        FABRIKSolver& operator=(FABRIKSolver&&) noexcept = default;
    
        void solve(chs::common::Chain& chain, const chs::common::Effector& end_effector) const;
        void solve(chs::common::Chain& chain, const chs::common::Effector& end_effector, const std::vector<chs::common::Effector>& middle_effectors) const;

    private:
        int num_of_iterations{1};

        [[nodiscard]] bool effectorFartherThanChainLength(const chs::common::Chain& chain, const chs::common::Effector& end_effector) const;
        void performChainStraightening(chs::common::Chain& chain, const chs::common::Effector& end_effector) const;
        void performFABRIKIterations(chs::common::Chain& chain, const chs::common::Effector& end_effector, const std::vector<chs::common::Effector>& middle_effectors) const;
        [[nodiscard]] chs::common::Effector createEffectorFromChainOrigin(const chs::common::Chain& chain) const;
        void performForwardReachingPass(chs::common::Chain& chain, const chs::common::Effector& end_effector, const std::vector<chs::common::Effector>& middle_effectors) const;
        void performBackwardReachingPass(chs::common::Chain& chain, const chs::common::Effector& end_effector) const;
        glm::vec3 calculateToMiddleEffectorsDirection(const glm::vec3& from, const std::vector<chs::common::Effector>& middle_effectors) const;
    };
}
