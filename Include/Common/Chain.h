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

#include <vector>

#include "Common/Segment.h"

namespace chs::common
{
    class Chain
    {
    public:
        Chain() = default;

        Chain(const Chain&) = default;
        Chain& operator=(const Chain&) = default;
        Chain(Chain&&) noexcept = default;
        Chain& operator=(Chain&&) noexcept = default;

        void addNextSegment(Segment segment);
        void refreshLocalTransforms();
        void refreshWorldTransforms();

        [[nodiscard]] float length() const;
        [[nodiscard]] bool empty() const { return chain_segments.empty(); }
        [[nodiscard]] int size() const { return static_cast<int>(chain_segments.size()); }
        [[nodiscard]] Segment& first();
        [[nodiscard]] const Segment& first() const;
        [[nodiscard]] Segment& last();
        [[nodiscard]] const Segment& last() const;
        [[nodiscard]] Segment& at(int index) { return chain_segments.at(index); }
        [[nodiscard]] const Segment& at(int index) const { return chain_segments.at(index); }
        [[nodiscard]] std::vector<Segment>& segments() { return chain_segments; }
        [[nodiscard]] const std::vector<Segment>& segments() const { return chain_segments; }
        [[nodiscard]] glm::vec3 locationAt(float distance_from_origin) const;
        [[nodiscard]] glm::vec3 worldOrigin() const;

    private:
        std::vector<Segment> chain_segments;
    };
}
