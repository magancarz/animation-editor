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

#include "Common/ChainNormalizer.h"

namespace chs::common
{
    Chain ChainNormalizer::normalize(const Chain& chain) const
    {
        Chain normalized_chain{};

        const float chain_total_length = chain.length();
        for (const auto& segment : chain.segments())
        {
            normalized_chain.addNextSegment(normalizeSegment(chain_total_length, segment));
        }

        return normalized_chain;
    }

    Segment ChainNormalizer::normalizeSegment(
        const float chain_total_length,
        const Segment& segment) const
    {
        glm::mat4 segment_local_transform = segment.localTransform();
        static constexpr int TRANSLATION_COLUMN_INDEX = 3;
        const glm::vec3 normalized_translation =
            segment_local_transform[TRANSLATION_COLUMN_INDEX] / chain_total_length;
        segment_local_transform[TRANSLATION_COLUMN_INDEX] = glm::vec4{normalized_translation, 1.0f};

        return Segment{segment_local_transform};
    }
}
