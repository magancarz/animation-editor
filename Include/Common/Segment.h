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

#include <glm/glm.hpp>

#include "Segment.h"

namespace chs::common
{
    class Segment
    {
    public:
        Segment() = default;
        explicit Segment(const glm::mat4& world_transform);

        Segment(const Segment&) = delete;
        Segment& operator=(const Segment&) = delete;
        Segment(Segment&&) noexcept = default;
        Segment& operator=(Segment&&) noexcept = default;

        void addChild(std::unique_ptr<Segment> child);
        void setWorldTransform(const glm::mat4& transform);
        void setLocalTransform(const glm::mat4& transform);
        void forceWorldTransform(const glm::mat4& transform);
        void refresh();

        [[nodiscard]] bool hasParent() const { return parent_segment; }
        [[nodiscard]] Segment& parent();
        [[nodiscard]] const Segment& parent() const;
        [[nodiscard]] bool hasChild() const { return child_segment.get(); }
        [[nodiscard]] Segment& child();
        [[nodiscard]] const Segment& child() const;
        [[nodiscard]] glm::vec3 worldOrigin() const;
        [[nodiscard]] glm::vec3 worldEnd() const;
        [[nodiscard]] glm::vec3 worldDirection() const;
        [[nodiscard]] const glm::mat4& worldTransform() const { return world_transform; }
        [[nodiscard]] const glm::mat4& localTransform() const { return local_transform; }
        [[nodiscard]] float length() const;
        [[nodiscard]] std::unique_ptr<Segment> clone() const;

    private:
        [[nodiscard]] glm::mat4 getParentWorldTransform() const;
        [[nodiscard]] Segment& parentImpl() const;
        [[nodiscard]] Segment& childImpl() const;

        Segment* parent_segment{};
        std::unique_ptr<Segment> child_segment{};
        glm::mat4 world_transform{1.0f};
        glm::mat4 local_transform{1.0f};
    };
}
