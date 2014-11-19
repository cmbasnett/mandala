#pragma once

//mandala
#include "md5b.hpp"
#include "aabb.hpp"
#include "hash.hpp"

//std
#include <vector>

namespace mandala
{
	struct model_skeleton_t
	{
		struct bone_t
		{
			uint8_t parent_index = 0;
			vec3_t position;
			quat_t orientation;
		};

		static void interpolate(model_skeleton_t& skeleton, const model_skeleton_t& a, const model_skeleton_t& b, float32_t t);

		std::vector<bone_t> bones;
		std::vector<mat4_t> bone_matrices;
		aabb3_t aabb;
	};
}
