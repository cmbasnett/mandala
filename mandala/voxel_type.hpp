#pragma once

//mandala
#include "types.hpp"

namespace mandala
{
	struct voxel_type_t
	{
		voxel_type_t();

		std::string name;
		bool is_visible = false;
		bool is_passable = false;
	};
}
