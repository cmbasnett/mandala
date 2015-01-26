#pragma once

//std
#include <type_traits>
#include <memory>

//mandala
#include "types.hpp"

namespace mandala
{
	struct voxel_type_t;

	typedef uint8_t voxel_visibility_flag_type;

	enum : voxel_visibility_flag_type
	{
		voxel_visibility_flag_none = 0,
		voxel_visibility_flag_right = 1 << 0,
		voxel_visibility_flag_left = 1 << 1,
		voxel_visibility_flag_down = 1 << 2,
		voxel_visibility_flag_up = 1 << 3,
		voxel_visibility_flag_back = 1 << 4,
		voxel_visibility_flag_front = 1 << 5,
		voxel_visibility_flag_all = (voxel_visibility_flag_right | voxel_visibility_flag_left | voxel_visibility_flag_down | voxel_visibility_flag_up | voxel_visibility_flag_back | voxel_visibility_flag_front)
	};

	struct voxel_t
	{
		typedef vec3_i32_t location_type;

		location_type location;
		voxel_visibility_flag_type visibility_flags = voxel_visibility_flag_none;

		voxel_t();
		voxel_t(int32_t x, int32_t y, int32_t z);

		std::shared_ptr<voxel_type_t> type;
	};
}