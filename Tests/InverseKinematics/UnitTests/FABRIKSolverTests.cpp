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

#include <gtest/gtest.h>

#include <memory>

#include "TestUtils.h"

#include "InverseKinematics/FABRIKSolver.h"

TEST(FABRIKSolverTests, ShouldReturnEarlyWhenTheChainIsEmpty)
{
    // given
    chs::ik::FABRIKSolver fabrik_solver{};

    chs::ik::Chain chain{};
    chs::ik::Effector effector;

    // when
    fabrik_solver.solve(chain, effector);
}

TEST(FABRIKSolverTests, ShouldPerformChainStraighteningWhenTheEffectorIsTooFar)
{
    // given
    chs::ik::FABRIKSolver fabrik_solver{};

    auto first_segment = std::make_unique<chs::ik::Segment>(TestUtils::fromVector(glm::vec3{1.0f, 0.0f, 0.0f}));
    auto second_segment = std::make_unique<chs::ik::Segment>(TestUtils::fromVector(glm::vec3{2.0f, 0.0f, 0.0f}));
    auto third_segment = std::make_unique<chs::ik::Segment>(TestUtils::fromVector(glm::vec3{3.0f, 0.0f, 0.0f}));
    
    second_segment->addChild(std::move(third_segment));
    first_segment->addChild(std::move(second_segment));

    chs::ik::Chain chain{std::move(first_segment)};

    chs::ik::Effector effector{};
    effector.world_location = glm::vec3{3.0f, 3.0f, 0.0f};

    // when
    fabrik_solver.solve(chain, effector);

    // then
    glm::vec3 chain_direction = glm::normalize(effector.world_location);

    auto expected_first_segment = std::make_unique<chs::ik::Segment>(TestUtils::fromVector(chain_direction * 1.0f));
    auto expected_second_segment = std::make_unique<chs::ik::Segment>(TestUtils::fromVector(chain_direction * 2.0f));
    auto expected_third_segment = std::make_unique<chs::ik::Segment>(TestUtils::fromVector(chain_direction * 3.0f));
    
    expected_second_segment->addChild(std::move(expected_third_segment));
    expected_first_segment->addChild(std::move(expected_second_segment));

    chs::ik::Chain expected_chain{std::move(expected_first_segment)};

    TestUtils::expectEqual(chain, expected_chain);
}

TEST(FABRIKSolverTests, ShouldApplyFABRIKOnGivenChain)
{
    // given
    chs::ik::FABRIKSolver fabrik_solver{5};

    auto first_segment = std::make_unique<chs::ik::Segment>(TestUtils::fromVector(glm::vec3{1.0f, 0.0f, 0.0f}));
    auto second_segment = std::make_unique<chs::ik::Segment>(TestUtils::fromVector(glm::vec3{2.0f, 0.0f, 0.0f}));
    auto third_segment = std::make_unique<chs::ik::Segment>(TestUtils::fromVector(glm::vec3{3.0f, 0.0f, 0.0f}));
    
    second_segment->addChild(std::move(third_segment));
    first_segment->addChild(std::move(second_segment));

    chs::ik::Chain chain{std::move(first_segment)};

    chs::ik::Effector effector{};
    effector.world_location = glm::vec3{1.0f, 1.0f, 0.0f};

    // when
    fabrik_solver.solve(chain, effector);

    // then
    auto expected_first_segment = std::make_unique<chs::ik::Segment>(TestUtils::fromVector(glm::vec3{0.991437f, -0.13059f, 0.0f}));
    auto expected_second_segment = std::make_unique<chs::ik::Segment>(TestUtils::fromVector(glm::vec3{1.820574f, 0.428456f, 0.0f}));
    auto expected_third_segment = std::make_unique<chs::ik::Segment>(TestUtils::fromVector(glm::vec3{1.0f, 1.0f, 0.0f}));
    
    expected_second_segment->addChild(std::move(expected_third_segment));
    expected_first_segment->addChild(std::move(expected_second_segment));

    chs::ik::Chain expected_chain{std::move(expected_first_segment)};

    TestUtils::expectEqual(chain, expected_chain);
}
