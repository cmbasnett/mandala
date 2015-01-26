//mandala
#include "voxel.hpp"
#include "voxel_type.hpp"

namespace mandala
{
	voxel_t::voxel_t() :
		voxel_t(0, 0, 0)
	{
	}

	voxel_t::voxel_t(int32_t x, int32_t y, int32_t z) :
		location(x, y, z)
	{
	}
}
