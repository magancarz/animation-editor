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
    Chain::Chain(std::unique_ptr<Segment> chain_root)
        : chain_root{std::move(chain_root)} {}

    float Chain::length() const
    {
        if (!chain_root)
        {
            return 0.0f;
        }

        float total_length = chain_root->length();
        Segment* current_segment = chain_root.get();
        while (current_segment->hasChild())
        {
            Segment* next_segment = &current_segment->child();
            total_length += next_segment->length();
            current_segment = next_segment;
        }

        return total_length;
    }

    Segment& Chain::first()
    {
        return firstImpl();
    }

    Segment& Chain::firstImpl() const
    {
        assert(chain_root && "Cannot call 'first' while the chain is empty!");
        return *chain_root;
    }

    const Segment& Chain::first() const
    {
        return firstImpl();
    }

    Segment& Chain::last()
    {
        return lastImpl();
    }

    Segment& Chain::lastImpl() const
    {
        assert(chain_root && "Cannot call 'last' while the chain is empty!");

        Segment* current_segment = chain_root.get();
        while (current_segment->hasChild())
        {
            Segment* next_segment = &current_segment->child();
            current_segment = next_segment;
        }

        return *current_segment;
    }

    const Segment& Chain::last() const
    {
        return lastImpl();
    }
}
