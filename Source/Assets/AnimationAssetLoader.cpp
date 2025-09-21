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

#include "Assets/AnimationAssetLoader.h"

#include <algorithm>

namespace chs::assets
{
    ImportedAssets AnimationAssetLoader::importAssets(
        const std::vector<SkeletonAssetInfo>& skeleton_asset_infos,
        const std::filesystem::path& file_path) const
    {
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(file_path, aiProcess_PopulateArmatureData);

        if (nullptr == scene)
        {
            printf("Assimp failed with %s\n", importer.GetErrorString());
            return {};
        }

        return importAssetsImpl(skeleton_asset_infos, scene);
    }

    ImportedAssets AnimationAssetLoader::importAssetsImpl(
        const std::vector<SkeletonAssetInfo>& skeleton_asset_infos,
        const aiScene* scene) const
    {
        ImportedAssets imported_assets{};
        imported_assets.skeletons = searchForSkeletons(skeleton_asset_infos, scene->mRootNode);

        return imported_assets;
    }

    std::vector<chs::common::Skeleton> AnimationAssetLoader::searchForSkeletons(
        const std::vector<SkeletonAssetInfo>& skeleton_asset_infos,
        const aiNode* current_node) const
    {
        if (!current_node)
        {
            return {};
        }

        if (std::optional<SkeletonAssetInfo> found_matching_info = findMatchingSkeletonAssetInfo(
                skeleton_asset_infos,
                current_node);
            found_matching_info.has_value())
        {
            return {importSkeleton(found_matching_info.value(), current_node)};
        }

        std::vector<chs::common::Skeleton> found_skeletons{};
        for (int child_index = 0; child_index < current_node->mNumChildren; ++child_index)
        {
            auto skeletons_to_insert = searchForSkeletons(
                skeleton_asset_infos,
                current_node->mChildren[child_index]);
            found_skeletons.insert(
                found_skeletons.end(),
                skeletons_to_insert.begin(),
                skeletons_to_insert.end());
        }

        return found_skeletons;
    }

    std::optional<SkeletonAssetInfo> AnimationAssetLoader::findMatchingSkeletonAssetInfo(
        const std::vector<SkeletonAssetInfo>& skeleton_asset_infos,
        const aiNode* current_node) const
    {
        std::string current_node_name = importName(current_node->mName);
        for (const auto& skeleton_asset_info : skeleton_asset_infos)
        {
            if (skeleton_asset_info.root_node_name == current_node_name)
            {
                return skeleton_asset_info;
            }
        }

        return std::nullopt;
    }

    chs::common::Skeleton AnimationAssetLoader::importSkeleton(
        const SkeletonAssetInfo& skeleton_asset_info,
        const aiNode* skeleton_root_node) const
    {
        std::string node_name = importName(skeleton_root_node->mName);

        std::vector<chs::common::Segment> imported_segments{};
        static constexpr glm::mat4 parent_world_transform{1.0f};
        std::ignore = traverseSkeleton(
            skeleton_asset_info,
            imported_segments,
            parent_world_transform,
            skeleton_root_node);
        return chs::common::Skeleton{skeleton_asset_info.skeleton_name, std::move(imported_segments)};
    }

    std::string AnimationAssetLoader::importName(const aiString& ai_string) const
    {
        std::string imported_name{};
        imported_name.resize(ai_string.length);
        memcpy(imported_name.data(), ai_string.data, ai_string.length);
        return imported_name;
    }

    int AnimationAssetLoader::traverseSkeleton(
        const SkeletonAssetInfo& skeleton_asset_info,
        std::vector<chs::common::Segment>& result_segments,
        const glm::mat4& parent_world_transform,
        const aiNode* current_skeleton_node) const
    {
        chs::common::Segment segment{};
        std::string node_name = importName(current_skeleton_node->mName);
        segment.setName(removePrefix(node_name, skeleton_asset_info.skeleton_prefix.size()));
        segment.setLocalTransform(importTransform(current_skeleton_node->mTransformation));
        segment.setWorldTransform(parent_world_transform * segment.localTransform());

        auto current_segment_index = static_cast<int>(result_segments.size());
        result_segments.emplace_back(std::move(segment));

        std::vector<int> child_indices{};
        child_indices.reserve(current_skeleton_node->mNumChildren);
        for (int child_index = 0; child_index < current_skeleton_node->mNumChildren; ++child_index)
        {
            child_indices.emplace_back(traverseSkeleton(
                skeleton_asset_info,
                result_segments,
                segment.worldTransform(),
                current_skeleton_node->mChildren[child_index]));
        }

        chs::common::Segment& current_segment = result_segments.at(current_segment_index);
        for (int child_index : child_indices)
        {
            current_segment.addChildSegmentIndex(child_index);
        }

        return current_segment_index;
    }

    std::string AnimationAssetLoader::removePrefix(std::string name, int prefix_size) const
    {
        return name.replace(0, prefix_size, "");
    }

    glm::mat4 AnimationAssetLoader::importTransform(const aiMatrix4x4& ai_transform) const
    {
        glm::mat4 result_matrix{};
        result_matrix[0] = glm::vec4{ai_transform.a1, ai_transform.b1, ai_transform.c1, ai_transform.d1};
        result_matrix[1] = glm::vec4{ai_transform.a2, ai_transform.b2, ai_transform.c2, ai_transform.d2};
        result_matrix[2] = glm::vec4{ai_transform.a3, ai_transform.b3, ai_transform.c3, ai_transform.d3};
        result_matrix[3] = glm::vec4{ai_transform.a4, ai_transform.b4, ai_transform.c4, ai_transform.d4};
        return result_matrix;
    }
}
