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

#include "Common/Chain.h"

#include <iostream>

namespace chs::common
{
    void Chain::addNextSegment(Segment segment)
    {
        if (!empty())
        {
            Segment& last_segment = last();
            last_segment.addChildSegmentIndex(static_cast<int>(chain_segments.size()));
            segment.setWorldTransform(last_segment.worldTransform() * segment.localTransform());
        }
        chain_segments.emplace_back(std::move(segment));
    }

    void Chain::refreshLocalTransforms()
    {
        glm::mat4 previous_world_transform{1.0f};
        for (auto& segment : chain_segments)
        {
            segment.setLocalTransform(glm::inverse(previous_world_transform) * segment.worldTransform());
            previous_world_transform = segment.worldTransform();
        }
    }

    void Chain::refreshWorldTransforms()
    {
        glm::mat4 previous_world_transform{1.0f};
        for (auto& segment : chain_segments)
        {
            segment.setWorldTransform(previous_world_transform * segment.localTransform());
            previous_world_transform = segment.worldTransform();
        }
    }

    float Chain::length() const
    {
        if (empty())
        {
            return 0.0f;
        }

        float total_length = 0.0f;
        for (const auto& segment : chain_segments)
        {
            total_length += segment.length();
        }

        return total_length;
    }

    Segment& Chain::first()
    {
        return chain_segments.front();
    }

    const Segment& Chain::first() const
    {
        return chain_segments.front();
    }

    Segment& Chain::last()
    {
        return chain_segments.back();
    }

    const Segment& Chain::last() const
    {
        return chain_segments.back();
    }

    glm::vec3 Chain::locationAt(float distance_from_origin) const
    {
        if (empty())
        {
            return glm::vec3{0.0f};
        }

        float current_distance = 0.0f;
        for (const auto& segment : chain_segments)
        {
            const float current_segment_length = segment.length();
            current_distance += current_segment_length;
            if (current_distance >= distance_from_origin)
            {
                const float segment_base_distance = current_distance - current_segment_length;
                const float segment_part = (distance_from_origin - segment_base_distance) / current_segment_length;
                const glm::vec3 segment_direction = segment.worldDirection();
                return segment.worldOrigin() + segment_direction * segment_part;
            }
        }

        return last().worldEnd();
    }

    glm::vec3 Chain::worldOrigin() const
    {
        if (empty())
        {
            return glm::vec3{0.0f};
        }

        return first().worldOrigin();
    }
}
