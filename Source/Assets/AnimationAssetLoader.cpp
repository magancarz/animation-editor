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

#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace chs::assets
{
    ImportedAssets AnimationAssetLoader::importAssets(
        const std::vector<SkeletonAssetInfo>& skeleton_asset_infos,
        const std::vector<AnimationAssetInfo>& animations_asset_infos,
        const std::filesystem::path& file_path) const
    {
        Assimp::Importer importer;

        importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

        const aiScene* scene = importer.ReadFile(file_path, aiProcess_PopulateArmatureData);

        if (nullptr == scene)
        {
            printf("Assimp failed with %s\n", importer.GetErrorString());
            return {};
        }

        return importAssetsImpl(skeleton_asset_infos, animations_asset_infos, scene);
    }

    ImportedAssets AnimationAssetLoader::importAssetsImpl(
        const std::vector<SkeletonAssetInfo>& skeleton_asset_infos,
        const std::vector<AnimationAssetInfo>& animations_asset_infos,
        const aiScene* scene) const
    {
        ImportedAssets imported_assets{};
        imported_assets.skeletons = searchForSkeletons(skeleton_asset_infos, scene->mRootNode);
        imported_assets.animations = importAnimations(animations_asset_infos, scene);

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

    std::vector<chs::anim::Animation> AnimationAssetLoader::importAnimations(
        const std::vector<AnimationAssetInfo>& animation_asset_infos,
        const aiScene* scene) const
    {
        if (!scene->HasAnimations())
        {
            return {};
        }

        std::vector<chs::anim::Animation> found_animations;
        for (int index = 0; index < scene->mNumAnimations; ++index)
        {
            const aiAnimation* ai_animation = scene->mAnimations[index];
            std::optional<AnimationAssetInfo> animation_asset_info = 
                findMatchingAnimationAssetInfo(animation_asset_infos, ai_animation);
            if (animation_asset_info.has_value())
            {
                found_animations.emplace_back(
                    importAnimation(
                        animation_asset_info.value(),
                        ai_animation));
            }
        }

        return found_animations;
    }

    std::optional<AnimationAssetInfo> AnimationAssetLoader::findMatchingAnimationAssetInfo(
        const std::vector<AnimationAssetInfo>& animation_asset_infos,
        const aiAnimation* animation) const
    {
        std::string animation_name = importName(animation->mName);
        for (const auto& animation_asset_info : animation_asset_infos)
        {
            if (animation_asset_info.animation_name == animation_name)
            {
                return animation_asset_info;
            }
        }

        return std::nullopt;
    }

    chs::anim::Animation AnimationAssetLoader::importAnimation(
        const AnimationAssetInfo& animation_asset_info,
        const aiAnimation* ai_animation) const
    {
        chs::anim::Animation animation{};
        animation.name = animation_asset_info.mapped_name;
        animation.duration = static_cast<float>(ai_animation->mDuration);
        animation.frames_per_second = static_cast<float>(ai_animation->mTicksPerSecond);
        animation.num_of_frames = static_cast<int>(std::round(animation.duration * animation.frames_per_second));
        animation.num_of_segments = ai_animation->mNumChannels;

        for (int channel_index = 0; channel_index < ai_animation->mNumChannels; ++channel_index)
        {
            chs::anim::AnimationChannel imported_channel = importAnimationChannel(
                animation,
                ai_animation->mChannels[channel_index]);
            std::string target_segment_name = imported_channel.segment_name;
            animation.channels.try_emplace(std::move(target_segment_name), std::move(imported_channel));
        }
        
        return animation;
    }

    chs::anim::AnimationChannel AnimationAssetLoader::importAnimationChannel(
        const chs::anim::Animation& target_animation,
        const aiNodeAnim* ai_animation_channel) const
    {
        chs::anim::AnimationChannel animation_channel{};
        animation_channel.segment_name = importName(ai_animation_channel->mNodeName);

        unsigned int position_key_index = std::min(1u, ai_animation_channel->mNumPositionKeys - 1);
        aiVectorKey current_position_key = ai_animation_channel->mPositionKeys[0];
        aiVectorKey next_position_key = ai_animation_channel->mPositionKeys[position_key_index];

        unsigned int rotation_key_index = std::min(1u, ai_animation_channel->mNumRotationKeys - 1);
        aiQuatKey current_rotation_key = ai_animation_channel->mRotationKeys[0];
        aiQuatKey next_rotation_key = ai_animation_channel->mRotationKeys[rotation_key_index];

        const float frame_delta_time = 1.0f / target_animation.frames_per_second;
        float current_time = 0.0f;
        animation_channel.key_frames.reserve(target_animation.num_of_frames);
        while (current_time < target_animation.duration)
        {
            if (position_key_index != ai_animation_channel->mNumPositionKeys)
            {
                aiVectorKey position_key = ai_animation_channel->mPositionKeys[position_key_index + 1];
                if (position_key.mTime <= current_time)
                {
                    current_position_key = next_position_key;
                    next_position_key = position_key;
                    position_key_index += 1;
                }
            }
            auto position_interpolation_t = static_cast<float>((current_time - current_position_key.mTime) /
                (next_position_key.mTime - current_position_key.mTime));
            aiVector3D interpolated_position{};
            Assimp::Interpolator<aiVectorKey>{}(
                interpolated_position,
                current_position_key,
                next_position_key,
                position_interpolation_t);

            glm::vec3 position{interpolated_position.x, interpolated_position.y, interpolated_position.z};
            glm::mat4 translation = glm::translate(glm::mat4{1.0f}, position);

            if (rotation_key_index != ai_animation_channel->mNumRotationKeys)
            {
                aiQuatKey rotation_key = ai_animation_channel->mRotationKeys[rotation_key_index + 1];
                if (rotation_key.mTime <= current_time)
                {
                    current_rotation_key = next_rotation_key;
                    next_rotation_key = rotation_key;
                    rotation_key_index += 1;
                }
            }
            auto rotation_interpolation_t = static_cast<float>((current_time - current_rotation_key.mTime) /
                (next_rotation_key.mTime - current_rotation_key.mTime));
            aiQuaternion interpolated_rotation{};
            Assimp::Interpolator<aiQuatKey>{}(
                interpolated_rotation,
                current_rotation_key,
                next_rotation_key,
                rotation_interpolation_t);

            glm::quat rotation{};
            rotation.x = interpolated_rotation.x;
            rotation.y = interpolated_rotation.y;
            rotation.z = interpolated_rotation.z;
            rotation.w = interpolated_rotation.w;

            chs::anim::KeyFrame key_frame{};
            key_frame.time = current_time;
            key_frame.local_transform = translation * glm::toMat4(rotation);
            animation_channel.key_frames.emplace_back(key_frame);

            current_time += frame_delta_time;
        }

        return animation_channel;
    }
}
