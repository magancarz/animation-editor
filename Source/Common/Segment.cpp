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

#include "Common/Segment.h"

namespace chs::common
{
    Segment::Segment(const glm::mat4& local_transform)
        : world_transform{local_transform}, local_transform{local_transform} {}
    
    Segment::Segment(std::string name, const glm::mat4& local_transform)
        : segment_name{std::move(name)}, world_transform{local_transform}, local_transform{local_transform} {}

    void Segment::addChildSegmentIndex(int child_segment_index)
    {
        child_segments_indices.emplace_back(child_segment_index);
    }

    void Segment::setName(const std::string& name)
    {
        segment_name = std::move(name);
    }

    void Segment::setWorldTransform(const glm::mat4& transform)
    {
        world_transform = transform;
    }

    void Segment::setLocalTransform(const glm::mat4& transform)
    {
        local_transform = transform;
    }

    glm::vec3 Segment::worldOrigin() const
    {
        return world_transform * glm::inverse(local_transform) * glm::vec4{0.0f, 0.0f, 0.0f, 1.0f};
    }

    glm::vec3 Segment::worldEnd() const
    {
        return world_transform * glm::vec4{0.0f, 0.0f, 0.0f, 1.0f};
    }

    glm::vec3 Segment::worldDirection() const
    {
        return worldEnd() - worldOrigin();
    }

    float Segment::length() const
    {
        static constexpr int TRANSLATION_COLUMN_INDEX = 3;
        return glm::length(glm::vec3{local_transform[TRANSLATION_COLUMN_INDEX]});
    }
}
