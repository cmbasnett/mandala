#pragma once

//mandala
#include "../gui_state.hpp"
#include "../voxel_map.hpp"

namespace mandala
{
	namespace armada
	{
		struct voxel_state_t : gui_state_t
		{
			voxel_map_t<uint32_t, 8> voxel_map;
		};
	}
}