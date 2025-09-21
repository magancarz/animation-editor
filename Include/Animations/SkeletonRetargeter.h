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

#include "Common/Chain.h"
#include "SkeletonRetargetingConfig.h"
#include "Skeleton.h"

namespace chs::anim
{
    class SkeletonRetargeter
    {
    public:
        SkeletonRetargeter() = default;
        explicit SkeletonRetargeter(SkeletonRetargetingConfig skeleton_retargeting_config);

        SkeletonRetargeter(const SkeletonRetargeter&) = default;
        SkeletonRetargeter& operator=(const SkeletonRetargeter&) = default;
        SkeletonRetargeter(SkeletonRetargeter&&) noexcept = default;
        SkeletonRetargeter& operator=(SkeletonRetargeter&&) noexcept = default;

        void retarget(const Skeleton& source_skeleton, Skeleton& target_skeleton) const;

    private:
        SkeletonRetargetingConfig skeleton_retargeting_config{};

        void retargetChain(
            const ChainMapping& chain_mapping,
            const Skeleton& source_skeleton,
            Skeleton& target_skeleton) const;
        [[nodiscard]] chs::common::Chain extractChainFromSkeleton(
            const std::string& first_segment_name,
            const std::string& last_segment_name,
            const Skeleton& skeleton,
            const glm::mat4& chain_inverse_transform) const;
        void applyRetargetedChainOntoSkeleton(
            const chs::common::Chain& retargeted_chain,
            Skeleton& skeleton) const;
    };
}
