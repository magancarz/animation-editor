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

#include "Common/Skeleton.h"

namespace chs::common
{
    Skeleton::Skeleton(std::vector<Segment> segments)
        : skeleton_segments{std::move(segments)},
        name_to_index_mappings{findSegmentsMappings(skeleton_segments)} {}

    std::unordered_map<std::string, int> Skeleton::findSegmentsMappings(
        const std::vector<Segment>& segments) const
    {
        std::unordered_map<std::string, int> result_mappings{};
        for (int segment_index = 0; segment_index < segments.size(); ++segment_index)
        {
            const auto& segment = segments.at(segment_index);
            result_mappings[segment.name()] = segment_index;
        }

        return result_mappings;
    }

    void Skeleton::refreshWorldTransforms()
    {
        static constexpr glm::mat4 IDENTITY_MATRIX{1.0f};
        refreshWorldTransformsImpl(IDENTITY_MATRIX, ROOT_SEGMENT_INDEX);
    }

    void Skeleton::refreshWorldTransformsImpl(
        const glm::mat4& parent_world_transform,
        int current_segment_index)
    {
        Segment& current_segment = skeleton_segments.at(current_segment_index);
        current_segment.setWorldTransform(parent_world_transform * current_segment.localTransform());
        for (int child_segment_index : current_segment.childSegmentIndices())
        {
            refreshWorldTransformsImpl(current_segment.worldTransform(), child_segment_index);
        }
    }

    int Skeleton::indexOf(const std::string& segment_name) const
    {
        assert(name_to_index_mappings.contains(segment_name) && "Given segment must exist!");
        return name_to_index_mappings.at(segment_name);
    }
}
