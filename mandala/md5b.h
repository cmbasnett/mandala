#pragma once

#include "types.h"

namespace mandala
{
	namespace md5b
	{
        static const auto magic_length = 4;
        static const auto magic = "MD5B";
        static const auto animation_magic = "ANIM";
        static const auto animation_version = 1;
        static const auto bone_null_index = 255;
        static const auto model_magic = "MESH";
        static const auto model_version = 1;

		inline void compute_quaternion_w(quat_t& quaternion)
		{
			auto t = 1.0f - ((quaternion.x * quaternion.x) + (quaternion.y * quaternion.y) + (quaternion.z * quaternion.z));

			if (t < 0.0f)
				quaternion.w = 0.0f;
			else
				quaternion.w = -glm::sqrt(t);
		}
	};
};