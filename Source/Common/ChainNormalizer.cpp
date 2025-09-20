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
        return recursivelyNormalizeChain(chain);
    }

    Chain ChainNormalizer::recursivelyNormalizeChain(const Chain& chain) const
    {
        float total_chain_length = chain.length();
        std::unique_ptr<Segment> normalized_chain_root = recursivelyNormalizeChain(
            total_chain_length, chain.first());

        return Chain{std::move(normalized_chain_root)};
    }

    std::unique_ptr<Segment> ChainNormalizer::recursivelyNormalizeChain(
        const float chain_total_length,
        const Segment& segment) const
    {
        auto normalized_segment = normalizeSegment(chain_total_length, segment);
        
        if (segment.hasChild())
        {
            normalized_segment->addChild(
                recursivelyNormalizeChain(chain_total_length, segment.child()));
        }

        return normalized_segment;
    }

    std::unique_ptr<Segment> ChainNormalizer::normalizeSegment(
        const float chain_total_length,
        const Segment& segment) const
    {
        glm::mat4 segment_local_transform = segment.localTransform();
        static constexpr int TRANSLATION_COLUMN_INDEX = 3;
        const glm::vec3 normalized_translation =
            segment_local_transform[TRANSLATION_COLUMN_INDEX] / chain_total_length;
        segment_local_transform[TRANSLATION_COLUMN_INDEX] = glm::vec4{normalized_translation, 1.0f};

        auto normalized_segment = segment.clone();
        normalized_segment->setWorldTransform(segment_local_transform);
        normalized_segment->setLocalTransform(segment_local_transform);
        
        return normalized_segment;
    }
}
