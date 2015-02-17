#pragma once

//mandala
#include "pose.hpp"
#include "model_skeleton.hpp"
#include "camera.hpp"

//std
#include <memory>

namespace mandala
{
	struct model_info_t;
	struct model_animation_t;

	struct model_t
	{
		model_t(std::shared_ptr<model_info_t> model);
        model_t(const hash_t& model_hash);

        pose3 pose;
		const std::shared_ptr<model_info_t> model;

		void tick(float32_t dt);
		void render(const camera_t& camera, const vec3_t& light_location) const;

        void play(const hash_t& animation_hash);

		const pose3 get_bone_pose(const hash_t& bone_hash) const;

    private:
        float32_t t = 0.0f;
        std::vector<mat4_t> bone_matrices;
		model_skeleton_t skeleton;
        std::shared_ptr<model_animation_t> animation;
	};
}
