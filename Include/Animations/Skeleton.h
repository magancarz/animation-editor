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

#include <unordered_map>

#include "Common/Chain.h"

namespace chs::anim
{
    class Skeleton
    {
    public:
        static constexpr int INVALID_SEGMENT_INDEX = -1;
        static constexpr int ROOT_SEGMENT_INDEX = 0;

        Skeleton() = default;
        explicit Skeleton(std::vector<chs::common::Segment> segments);

        void refreshWorldTransforms();

        [[nodiscard]] bool empty() const { return skeleton_segments.empty(); }
        [[nodiscard]] int size() const { return static_cast<int>(skeleton_segments.size()); }
        [[nodiscard]] chs::common::Segment& at(int index) { return skeleton_segments.at(index); }
        [[nodiscard]] const chs::common::Segment& at(int index) const { return skeleton_segments.at(index); }
        [[nodiscard]] std::vector<chs::common::Segment>& segments() { return skeleton_segments; }
        [[nodiscard]] const std::vector<chs::common::Segment>& segments() const { return skeleton_segments; }
        [[nodiscard]] int indexOf(const std::string& segment_name) const;

    private:
        [[nodiscard]] std::unordered_map<std::string, int> findSegmentsMappings(
            const std::vector<chs::common::Segment>& skeleton_segments) const;

        std::vector<chs::common::Segment> skeleton_segments{};
        std::unordered_map<std::string, int> name_to_index_mappings{};

        void refreshWorldTransformsImpl(
            const glm::mat4& parent_world_transform,
            int current_segment_index);
    };
}
