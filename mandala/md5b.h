#pragma once

#include "types.h"

#define MD5B_MAGIC_LENGTH				(4)
#define MD5B_MAGIC						("MD5B")
#define MD5B_ANIMATION_MAGIC			("ANIM")
#define MD5B_ANIMATION_VERSION			(1)
#define MD5B_BONE_NULL_INDEX			(255)
#define MD5B_MODEL_MAGIC				("MESH")
#define MD5B_MODEL_VERSION				(1)

namespace mandala
{
	namespace md5b
	{
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