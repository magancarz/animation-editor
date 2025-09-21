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

#include "Animations/SkeletonRetargeter.h"

#include "Animations/ChainRetargeter.h"

namespace chs::anim
{
    SkeletonRetargeter::SkeletonRetargeter(SkeletonRetargetingConfig skeleton_retargeting_config)
        : skeleton_retargeting_config{std::move(skeleton_retargeting_config)} {}

    void SkeletonRetargeter::retarget(const chs::common::Skeleton& source_skeleton, chs::common::Skeleton& target_skeleton) const
    {
        for (const auto& chain_mapping : skeleton_retargeting_config.chain_mappings)
        {
            retargetChain(chain_mapping, source_skeleton, target_skeleton);
        }
    }

    void SkeletonRetargeter::retargetChain(
            const ChainMapping& chain_mapping,
            const chs::common::Skeleton& source_skeleton,
            chs::common::Skeleton& target_skeleton) const
    {
        int first_segment_index = target_skeleton.indexOf(chain_mapping.target_first_segment);
        const chs::common::Segment& base_segment = target_skeleton.at(first_segment_index);
        glm::mat4 chain_inverse_transform = glm::inverse(base_segment.worldTransform() *
            glm::inverse(base_segment.localTransform()));

        chs::common::Chain source_chain = extractChainFromSkeleton(
            chain_mapping.source_first_segment,
            chain_mapping.source_last_segment,
            source_skeleton,
            chain_inverse_transform);

        chs::common::Chain target_chain = extractChainFromSkeleton(
            chain_mapping.target_first_segment,
            chain_mapping.target_last_segment,
            target_skeleton,
            chain_inverse_transform);

        ChainRetargeter chain_retargeter{};
        chain_retargeter.retarget(source_chain, target_chain);
        applyRetargetedChainOntoSkeleton(target_chain, target_skeleton);
    }

    chs::common::Chain SkeletonRetargeter::extractChainFromSkeleton(
        const std::string& first_segment_name,
        const std::string& last_segment_name,
        const chs::common::Skeleton& skeleton,
        const glm::mat4& chain_inverse_transform) const
    {
        int first_segment_index = skeleton.indexOf(first_segment_name);
        int last_segment_index = skeleton.indexOf(last_segment_name);
        assert(last_segment_index - first_segment_index >= 0 &&
            "Chain must consist of at least one segment");

        chs::common::Chain extracted_chain{};

        for (int segment_index = first_segment_index; segment_index <= last_segment_index; ++segment_index)
        {
            chs::common::Segment extracted_segment = skeleton.at(segment_index);
            extracted_segment.setWorldTransform(chain_inverse_transform * extracted_segment.worldTransform());
            extracted_chain.addNextSegment(std::move(extracted_segment));
        }

        return extracted_chain;
    }

    void SkeletonRetargeter::applyRetargetedChainOntoSkeleton(
        const chs::common::Chain& retargeted_chain,
        chs::common::Skeleton& skeleton) const
    {
        int first_segment_index = skeleton.indexOf(retargeted_chain.first().name());
        int last_segment_index = skeleton.indexOf(retargeted_chain.last().name());
        for (int segment_index = 0; segment_index <= last_segment_index - first_segment_index; ++segment_index)
        {
            const chs::common::Segment& retargeted_segment = retargeted_chain.at(segment_index);
            chs::common::Segment& target_segment = skeleton.at(first_segment_index + segment_index);
            target_segment.setLocalTransform(retargeted_segment.localTransform());
        }
        skeleton.refreshWorldTransforms();
    }
}
