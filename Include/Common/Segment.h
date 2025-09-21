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

#include <memory>
#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "Segment.h"

namespace chs::common
{
    class Segment
    {
    public:
        Segment() = default;
        explicit Segment(const glm::mat4& local_transform);
        Segment(std::string name, const glm::mat4& local_transform);

        Segment(const Segment&) = default;
        Segment& operator=(const Segment&) = default;
        Segment(Segment&&) noexcept = default;
        Segment& operator=(Segment&&) noexcept = default;

        void addChildSegmentIndex(int child_segment_index);
        void setName(const std::string& name);
        void setWorldTransform(const glm::mat4& transform);
        void setLocalTransform(const glm::mat4& transform);

        [[nodiscard]] const std::vector<int>& childSegmentIndices() const { return child_segments_indices; }
        [[nodiscard]] const std::string& name() const { return segment_name; }
        [[nodiscard]] glm::vec3 worldOrigin() const;
        [[nodiscard]] glm::vec3 worldEnd() const;
        [[nodiscard]] glm::vec3 worldDirection() const;
        [[nodiscard]] const glm::mat4& worldTransform() const { return world_transform; }
        [[nodiscard]] const glm::mat4& localTransform() const { return local_transform; }
        [[nodiscard]] float length() const;

    private:
        std::vector<int> child_segments_indices{};
        std::string segment_name{};
        glm::mat4 world_transform{1.0f};
        glm::mat4 local_transform{1.0f};
    };
}
