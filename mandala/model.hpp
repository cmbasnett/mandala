#pragma once

//mandala
#include "model_skeleton.hpp"
#include "model_animation_controller.hpp"
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

		void tick(float32_t dt);
		void render(const camera_t& camera, const vec3_t& light_position) const;

		mat4_t world_matrix;
		std::shared_ptr<model_info_t> model;
		float32_t t = 0.0f;
		std::vector<mat4_t> bone_matrices;
		model_skeleton_t skeleton;
		model_animation_controller_t animation_controller;

		const void get_bone(const hash_t& bone_name_hash, vec3_t& location, quat_t& rotation) const;
	};
}
