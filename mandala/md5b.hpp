#pragma once

#include "types.hpp"

namespace mandala
{
	namespace md5b
	{
        static const auto magic_length = 4;
        static const auto animation_magic = "MD5A";
        static const auto animation_version = 1;
        static const auto bone_null_index = 255;
        static const auto model_magic = "MD5M";
        static const auto model_version = 1;

		inline void compute_quaternion_w(quat_t& quaternion)
		{
			auto t = 1.0f - ((quaternion.x * quaternion.x) + (quaternion.y * quaternion.y) + (quaternion.z * quaternion.z));

			quaternion.w = t < 0.0f ? 0.0f : -glm::sqrt(t);
		}
	}
}
