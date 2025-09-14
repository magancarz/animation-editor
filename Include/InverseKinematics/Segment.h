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

#include <glm/glm.hpp>

namespace chs::ik
{
    class Segment
    {
    public:
        Segment() = default;
        Segment(const glm::vec3& world_origin, const glm::vec3& world_end);

        Segment(const Segment&) = default;
        Segment& operator=(const Segment&) = default;
        Segment(Segment&&) noexcept = default;
        Segment& operator=(Segment&&) noexcept = default;

        [[nodiscard]] glm::vec3 worldOrigin() const { return world_origin; }
        [[nodiscard]] glm::vec3 worldEnd() const { return world_end; }
        [[nodiscard]] float length() const { return segment_length; }

        void setWorldOrigin(const glm::vec3& origin);
        void setWorldEnd(const glm::vec3& end);
        void refresh();

    private:
        glm::vec3 world_origin{0};
        glm::vec3 world_end{0};
        float segment_length{0};
    };
}
