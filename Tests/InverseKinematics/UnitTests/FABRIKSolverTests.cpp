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

    chs::common::Chain chain{};
    chs::common::Effector effector{};

    // when
    fabrik_solver.solve(chain, effector);
}

TEST(FABRIKSolverTests, ShouldPerformChainStraighteningWhenTheEffectorIsTooFar)
{
    // given
    glm::mat4 root_transform = TestUtils::fromVector(glm::vec3{1.0f, 0.0f, 0.0f});
    glm::mat4 basic_transform = TestUtils::fromVector(glm::vec3{0.0f, 1.0f, 0.0f});

    chs::common::Chain chain{};
    chain.addNextSegment(chs::common::Segment{root_transform});
    chain.addNextSegment(chs::common::Segment{basic_transform});
    chain.addNextSegment(chs::common::Segment{basic_transform});
    
    chs::common::Effector effector{};
    effector.world_location = glm::vec3{3.0f, 3.0f, 0.0f};

    chs::ik::FABRIKSolver fabrik_solver{};

    // when
    fabrik_solver.solve(chain, effector);

    // then
    glm::vec3 chain_direction = glm::normalize(effector.world_location);

    glm::mat4 first_expected_world_transform = TestUtils::fromVector(chain_direction * 1.0f);
    glm::mat4 expected_local_transform = TestUtils::fromVector(glm::vec3{0.0f, 1.0f, 0.0f});

    chs::common::Chain expected_chain{};
    expected_chain.addNextSegment(chs::common::Segment{first_expected_world_transform});
    expected_chain.addNextSegment(chs::common::Segment{expected_local_transform});
    expected_chain.addNextSegment(chs::common::Segment{expected_local_transform});

    TestUtils::expectEqual(chain, expected_chain);
}

TEST(FABRIKSolverTests, ShouldApplyFABRIKOnGivenChain)
{
    // given
    chs::ik::FABRIKSolver fabrik_solver{5};

    glm::mat4 root_transform = TestUtils::fromVector(glm::vec3{1.0f, 0.0f, 0.0f});
    glm::mat4 basic_transform = TestUtils::fromVector(glm::vec3{0.0f, 1.0f, 0.0f});

    chs::common::Chain chain{};
    chain.addNextSegment(chs::common::Segment{root_transform});
    chain.addNextSegment(chs::common::Segment{basic_transform});
    chain.addNextSegment(chs::common::Segment{basic_transform});

    chs::common::Effector effector{};
    effector.world_location = glm::vec3{1.0f, 1.0f, 0.0f};

    // when
    fabrik_solver.solve(chain, effector);

    // then
    glm::mat4 first_expected_transform = TestUtils::fromVector(glm::vec3{0.991437f, -0.13059f, 0.0f});
    glm::mat4 second_expected_transform = TestUtils::fromVector(glm::vec3{1.820574f, 0.428456f, 0.0f});
    glm::mat4 third_expected_transform = TestUtils::fromVector(glm::vec3{1.0f, 1.0f, 0.0f});

    chs::common::Chain expected_chain{};
    expected_chain.addNextSegment(chs::common::Segment{first_expected_transform});
    expected_chain.addNextSegment(chs::common::Segment{glm::inverse(first_expected_transform) * second_expected_transform});
    expected_chain.addNextSegment(chs::common::Segment{glm::inverse(second_expected_transform) * third_expected_transform});

    TestUtils::expectEqual(chain, expected_chain);
}

TEST(FABRIKSolverTests, ShouldAffectFABRIKWithMiddleEffectors)
{
    // given
    chs::ik::FABRIKSolver fabrik_solver{10};

    glm::mat4 root_transform = TestUtils::fromVector(glm::vec3{1.0f, 0.0f, 0.0f});
    glm::mat4 basic_transform = TestUtils::fromVector(glm::vec3{0.0f, 1.0f, 0.0f});

    chs::common::Chain chain{};
    chain.addNextSegment(chs::common::Segment{root_transform});
    chain.addNextSegment(chs::common::Segment{basic_transform});
    chain.addNextSegment(chs::common::Segment{basic_transform});

    chs::common::Effector effector{};
    effector.world_location = glm::vec3{1.0f, 1.0f, 0.0f};

    chs::common::Effector middle_effector{};
    middle_effector.world_location = glm::vec3{-1.0f, 2.0f, 0.0f};

    std::vector<chs::common::Effector> middle_effectors{middle_effector};

    // when
    fabrik_solver.solve(chain, effector, middle_effectors);

    // then
    glm::mat4 first_expected_transform = TestUtils::fromVector(glm::vec3{-0.536611f, 0.843829f, 0.0f});
    glm::mat4 second_expected_transform = TestUtils::fromVector(glm::vec3{0.178203f, 1.543144f, 0.0f});
    glm::mat4 third_expected_transform = TestUtils::fromVector(glm::vec3{1.012459f, 0.991766f, 0.0f});

    chs::common::Chain expected_chain{};
    expected_chain.addNextSegment(chs::common::Segment{first_expected_transform});
    expected_chain.addNextSegment(chs::common::Segment{glm::inverse(first_expected_transform) * second_expected_transform});
    expected_chain.addNextSegment(chs::common::Segment{glm::inverse(second_expected_transform) * third_expected_transform});

    TestUtils::expectEqual(chain, expected_chain);
}
