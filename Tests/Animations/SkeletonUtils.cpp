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

#include "Animations/SkeletonUtils.h"

#include <gtest/gtest.h>

#include "TestUtils.h"

chs::anim::Skeleton SkeletonUtils::createSkeleton(const std::vector<SegmentEntry>& segment_entries)
{
    std::vector<chs::common::Segment> skeleton_segments{};
    skeleton_segments.reserve(segment_entries.size());
    chs::common::Segment* previous_segment = nullptr;
    for (const auto& segment_entry : segment_entries)
    {
        glm::mat4 previous_segment_world_transform{1.0f};
        if (previous_segment)
        {
            previous_segment->addChildSegmentIndex(skeleton_segments.size());
            previous_segment_world_transform = previous_segment->worldTransform();
        }
        glm::mat4 segment_world_transform = TestUtils::fromVector(segment_entry.segment_local_location);
        glm::mat4 segment_local_transform =
            glm::inverse(previous_segment_world_transform) * segment_world_transform;
        chs::common::Segment segment{segment_entry.segment_name, segment_local_transform};
        segment.setWorldTransform(segment_world_transform);
        previous_segment = &skeleton_segments.emplace_back(std::move(segment));
    }

    return chs::anim::Skeleton{std::move(skeleton_segments)};
}

void SkeletonUtils::expectEqual(const chs::anim::Skeleton& first, const chs::anim::Skeleton& second, float precision)
{
    EXPECT_EQ(first.size(), second.size());

    for (int segment_index = 0; segment_index < first.size(); ++segment_index)
    {
        TestUtils::expectEqual(first.at(segment_index), second.at(segment_index), precision);
    }
}
