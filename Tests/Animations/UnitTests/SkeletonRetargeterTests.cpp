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

#include "TestUtils.h"
#include "Animations/SkeletonRetargeter.h"

TEST(SkeletonRetargeterTests, ShouldRetargetTargetSkeletonFromGivenSourceSkeletonSimpleCase)
{
    // given
    std::vector<SegmentEntry> skeleton_segment_entries{};

    skeleton_segment_entries.emplace_back("spine0", glm::vec3{0, 1, 0});
    skeleton_segment_entries.emplace_back("spine1", glm::vec3{0, 2, 0});
    skeleton_segment_entries.emplace_back("spine2", glm::vec3{1, 2, 0});
    skeleton_segment_entries.emplace_back("arm0", glm::vec3{2, 2, 0});
    skeleton_segment_entries.emplace_back("arm1", glm::vec3{2, 3, 0});
    chs::common::Skeleton source_skeleton = TestUtils::createSkeleton(skeleton_segment_entries);

    skeleton_segment_entries.clear();
    skeleton_segment_entries.emplace_back("spine0", glm::vec3{1, 0, 0});
    skeleton_segment_entries.emplace_back("spine1", glm::vec3{2, 0, 0});
    skeleton_segment_entries.emplace_back("spine2", glm::vec3{3, 0, 0});
    skeleton_segment_entries.emplace_back("arm0", glm::vec3{4, 0, 0});
    skeleton_segment_entries.emplace_back("arm1", glm::vec3{5, 0, 0});
    chs::common::Skeleton target_skeleton = TestUtils::createSkeleton(skeleton_segment_entries);

    std::vector<chs::anim::ChainMapping> chain_mappings{};
    chain_mappings.emplace_back("spine0", "spine2", "spine0", "spine2");
    chain_mappings.emplace_back("spine2", "arm1", "spine2", "arm1");
    chs::anim::SkeletonRetargetingConfig skeleton_retargeting_config{std::move(chain_mappings)};

    chs::anim::SkeletonRetargeter skeleton_retargeter{std::move(skeleton_retargeting_config)};

    // when
    skeleton_retargeter.retarget(source_skeleton, target_skeleton);

    // then
    TestUtils::expectEqual(source_skeleton, target_skeleton);
}
