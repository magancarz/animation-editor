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

#include "Common/ChainNormalizer.h"

TEST(ChainNormalizerTests, ShouldNormalizeChain)
{
    // given
    glm::mat4 first_segment_world_transform = TestUtils::fromVector(glm::vec3{0.0f, 1.0f, 0.0f});
    glm::mat4 second_segment_world_transform = TestUtils::fromVector(glm::vec3{1.0f, 1.0f, 0.0f});
    glm::mat4 third_segment_world_transform = TestUtils::fromVector(glm::vec3{1.0f, 2.0f, 0.0f});

    chs::common::Chain chain{};
    chain.addNextSegment(chs::common::Segment{first_segment_world_transform});
    chain.addNextSegment(chs::common::Segment{glm::inverse(first_segment_world_transform) * second_segment_world_transform});
    chain.addNextSegment(chs::common::Segment{glm::inverse(second_segment_world_transform) * third_segment_world_transform});

    chs::common::ChainNormalizer chain_normalizer{};

    // when
    chs::common::Chain normalized_chain = chain_normalizer.normalize(chain);

    // then
    glm::mat4 expected_first_segment_world_transform = TestUtils::fromVector(glm::vec3{0.0f, 0.333333f, 0.0f});
    glm::mat4 expected_second_segment_world_transform = TestUtils::fromVector(glm::vec3{0.333333f, 0.333333f, 0.0f});
    glm::mat4 expected_third_segment_world_transform = TestUtils::fromVector(glm::vec3{0.333333f, 0.666666f, 0.0f});

    chs::common::Chain expected_chain{};
    expected_chain.addNextSegment(chs::common::Segment{expected_first_segment_world_transform});
    expected_chain.addNextSegment(chs::common::Segment{
        glm::inverse(expected_first_segment_world_transform) * expected_second_segment_world_transform});
    expected_chain.addNextSegment(chs::common::Segment{
        glm::inverse(expected_second_segment_world_transform) * expected_third_segment_world_transform});
}
