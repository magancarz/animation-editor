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

#include "Animations/ChainRetargeter.h"

#include <glm/glm.hpp>

#include "Common/ChainNormalizer.h"
#include "Common/ConstChainIterator.h"
#include "Common/ChainIterator.h"
#include "Common/LinearAlgebra.h"

namespace chs::anim
{
    void ChainRetargeter::retarget(const chs::common::Chain& source, chs::common::Chain& target) const
    {
        chs::common::ChainNormalizer chain_normalizer{};
        chs::common::Chain normalized_source_chain = chain_normalizer.normalize(source);
        chs::common::Chain normalized_target_chain = chain_normalizer.normalize(target);
        std::vector<chs::common::Effector> effectors = pickEffectorsFromSourceChain(
            normalized_source_chain, normalized_target_chain);
        applyForwardReachingPass(normalized_target_chain, effectors);
        normalized_target_chain.first().refresh();
        applyRetargetedChainOntoTarget(normalized_target_chain, target);
    }

    std::vector<chs::common::Effector> ChainRetargeter::pickEffectorsFromSourceChain(
        const chs::common::Chain& normalized_source_chain,
        const chs::common::Chain& normalized_target_chain) const
    {
        std::vector<chs::common::Effector> effectors{};
        float current_distance = 0.0f;
        chs::common::ConstChainIterator const_chain_iterator{normalized_target_chain};
        while (const_chain_iterator.hasNext())
        {
            const chs::common::Segment& segment = const_chain_iterator.next();
            current_distance += segment.length();
            chs::common::Effector effector{};
            effector.world_location = normalized_source_chain.locationAt(current_distance);
            effectors.emplace_back(effector);
        }

        return effectors;
    }

    void ChainRetargeter::applyForwardReachingPass(
        chs::common::Chain& normalized_target_chain,
        const std::vector<chs::common::Effector>& effectors) const
    {
        glm::vec3 previous_segment_end = normalized_target_chain.worldOrigin();

        int current_segment_index = 0;
        chs::common::ChainIterator chain_iterator{normalized_target_chain};
        while (chain_iterator.hasNext())
        {
            chs::common::Segment& current_segment = chain_iterator.next();
            const float current_segment_length = current_segment.length();
            const chs::common::Effector& current_effector = effectors.at(current_segment_index);
            const glm::vec3 to_world_end_direction = glm::safeNormalize(current_effector.world_location - previous_segment_end);
            const glm::vec3 current_segment_world_end = previous_segment_end + to_world_end_direction * current_segment_length;
            const glm::mat4 current_segment_world_translation = glm::translate(glm::mat4{1.0f}, current_segment_world_end);
            const glm::mat4 current_segment_world_rotation = glm::toMat4(glm::quat{glm::vec3{0, 1, 0}, to_world_end_direction});
            current_segment.forceWorldTransform(current_segment_world_translation * current_segment_world_rotation);
            previous_segment_end = current_segment_world_end;
            current_segment_index += 1;
        }
    }

    void ChainRetargeter::applyRetargetedChainOntoTarget(
        const chs::common::Chain& normalized_target_chain,
        chs::common::Chain& target_chain) const
    {
        chs::common::ConstChainIterator normalized_chain_iterator{normalized_target_chain};
        chs::common::ChainIterator chain_iterator{target_chain};

        while (normalized_chain_iterator.hasNext() && chain_iterator.hasNext())
        {
            const chs::common::Segment& normalized_segment = normalized_chain_iterator.next();
            chs::common::Segment& segment = chain_iterator.next();
            glm::mat4 segment_local_transform = normalized_segment.localTransform();
            static constexpr int TRANSLATION_COLUMN_INDEX = 3;
            const glm::vec3 translation = segment_local_transform[TRANSLATION_COLUMN_INDEX] * segment.length();
            segment_local_transform[TRANSLATION_COLUMN_INDEX] = glm::vec4{translation, 1.0f};
            segment.setLocalTransform(segment_local_transform);
        }
    }
}
