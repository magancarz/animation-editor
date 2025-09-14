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

#include "InverseKinematics/Segment.h"

namespace chs::ik
{
    class Chain
    {
    public:
        Chain() = default;

        Chain(const Chain&) = default;
        Chain& operator=(const Chain&) = default;
        Chain(Chain&&) noexcept = default;
        Chain& operator=(Chain&&) noexcept = default;
    
        [[nodiscard]] float calculateTotalLength() const;

        void addSegment(Segment segment);

        [[nodiscard]] std::vector<Segment>& segments() { return chain_segments; }
        [[nodiscard]] const std::vector<Segment>& segments() const { return chain_segments; }
        [[nodiscard]] Segment& at(std::size_t index) { return chain_segments.at(index); }
        [[nodiscard]] const Segment& at(std::size_t index) const { return chain_segments.at(index); }
        [[nodiscard]] int empty() const { return chain_segments.empty(); }
        [[nodiscard]] int size() const { return chain_segments.size(); }
        [[nodiscard]] Segment& first() { return chain_segments.front(); }
        [[nodiscard]] const Segment& first() const { return chain_segments.front(); }
        [[nodiscard]] Segment& last() { return chain_segments.back(); }
        [[nodiscard]] const Segment& last() const { return chain_segments.back(); }

    private:
        std::vector<Segment> chain_segments;
    };
}
