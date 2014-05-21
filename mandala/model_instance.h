#pragma once

//mandala
#include "skeleton.h"
#include "camera.h"

//std
#include <memory>

namespace mandala
{
	struct model_t;
	struct animation_t;

	struct model_instance_t
	{
		model_instance_t(std::shared_ptr<model_t> model);

		void tick(float32_t dt);
		void render(const camera_t& camera, const vec3_t& light_position) const;

		mat4_t world_matrix;
		std::shared_ptr<model_t> model;
		std::shared_ptr<animation_t> animation;
		skeleton_t skeleton;
		float32_t t = 0.0f;
		std::vector<mat4_t> bone_matrices;

		const skeleton_t::bone_t& get_bone(const hash_t& bone_name_hash) const;
	};
};
