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
        explicit Chain(std::unique_ptr<Segment> chain_root);

        Chain(const Chain&) = delete;
        Chain& operator=(const Chain&) = delete;
        Chain(Chain&&) noexcept = default;
        Chain& operator=(Chain&&) noexcept = default;

        [[nodiscard]] float length() const;
        [[nodiscard]] bool empty() const { return !chain_root; }
        [[nodiscard]] Segment& first();
        [[nodiscard]] const Segment& first() const;
        [[nodiscard]] Segment& last();
        [[nodiscard]] const Segment& last() const;

    private:
        Segment& firstImpl() const;
        Segment& lastImpl() const;

        std::unique_ptr<Segment> chain_root;
    };
}
