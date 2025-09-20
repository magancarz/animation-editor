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
#include "Animations/ChainRetargeter.h"

TEST(ChainRetargeterTests, ShouldRetargetTargetChainFromGivenSourceChain)
{
    // given
    glm::mat4 source_chain_first_segment_world_transform = TestUtils::fromVector(glm::vec3{0.0f, 1.0f, 0.0f});
    glm::mat4 source_chain_second_segment_world_transform = TestUtils::fromVector(glm::vec3{1.0f, 1.0f, 0.0f});

    chs::common::Chain source_chain{};
    source_chain.addNextSegment(chs::common::Segment{source_chain_first_segment_world_transform});
    source_chain.addNextSegment(chs::common::Segment{
        glm::inverse(source_chain_first_segment_world_transform) * source_chain_second_segment_world_transform});

    glm::mat4 target_chain_first_segment_world_transform = TestUtils::fromVector(glm::vec3{1.0f, 0.0f, 0.0f});
    glm::mat4 target_chain_second_segment_world_transform = TestUtils::fromVector(glm::vec3{2.0f, 0.0f, 0.0f});

    chs::common::Chain target_chain{};
    target_chain.addNextSegment(chs::common::Segment{target_chain_first_segment_world_transform});
    target_chain.addNextSegment(chs::common::Segment{
        glm::inverse(target_chain_first_segment_world_transform) * target_chain_second_segment_world_transform});

    chs::anim::ChainRetargeter chain_retargeter{};

    // when
    chain_retargeter.retarget(source_chain, target_chain);

    // then
    TestUtils::expectEqual(source_chain, target_chain);
}
