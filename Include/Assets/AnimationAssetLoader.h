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

#include <filesystem>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/anim.h>

#include <glm/glm.hpp>

#include "Common/Skeleton.h"
#include "Animations/Animation.h"
#include "ImportedAssets.h"
#include "SkeletonAssetInfo.h"
#include "AnimationAssetInfo.h"

namespace chs::assets
{
    class AnimationAssetLoader
    {
    public:
        [[nodiscard]] ImportedAssets importAssets(
            const std::vector<SkeletonAssetInfo>& skeleton_asset_infos,
            const std::vector<AnimationAssetInfo>& animations_asset_infos,
            const std::filesystem::path& file_path) const;

    private:
        [[nodiscard]] ImportedAssets importAssetsImpl(
            const std::vector<SkeletonAssetInfo>& skeleton_asset_infos,
            const std::vector<AnimationAssetInfo>& animations_asset_infos,
            const aiScene* scene) const;
        [[nodiscard]] std::vector<chs::common::Skeleton> searchForSkeletons(
            const std::vector<SkeletonAssetInfo>& skeleton_asset_infos,
            const aiNode* current_node) const;
        [[nodiscard]] std::optional<SkeletonAssetInfo> findMatchingSkeletonAssetInfo(
            const std::vector<SkeletonAssetInfo>& skeleton_asset_infos,
            const aiNode* current_node) const;
        [[nodiscard]] chs::common::Skeleton importSkeleton(
            const SkeletonAssetInfo& skeleton_asset_info,
            const aiNode* skeleton_root_node) const;
        [[nodiscard]] std::string importName(const aiString& ai_string) const;
        [[nodiscard]] int traverseSkeleton(
            const SkeletonAssetInfo& skeleton_asset_info,
            std::vector<chs::common::Segment>& result_segment,
            const glm::mat4& parent_world_transform,
            const aiNode* current_skeleton_node) const;
        [[nodiscard]] std::string removePrefix(std::string name, int prefix_size) const;
        [[nodiscard]] glm::mat4 importTransform(const aiMatrix4x4& ai_transform) const;
        [[nodiscard]] std::vector<chs::anim::Animation> importAnimations(
            const std::vector<AnimationAssetInfo>& animation_asset_infos,
            const aiScene* scene) const;
        [[nodiscard]] std::optional<AnimationAssetInfo> findMatchingAnimationAssetInfo(
            const std::vector<AnimationAssetInfo>& animation_asset_infos,
            const aiAnimation* animation) const;
        [[nodiscard]] chs::anim::Animation importAnimation(
            const AnimationAssetInfo& animation_asset_info,
            const aiAnimation* ai_animation) const;
        [[nodiscard]] chs::anim::AnimationChannel importAnimationChannel(
            const chs::anim::Animation& target_animation,
            const aiNodeAnim* ai_animation_channel) const;
    };
}
