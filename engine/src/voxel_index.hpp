#pragma once

// naga
#include "types.hpp"

namespace naga
{
	struct VoxelIndex
	{
		typedef vec3_u32 IndexType;

		IndexType chunk_index;
		IndexType index;

		bool is_valid() const;

		VoxelIndex north() const;
		VoxelIndex east() const;
		VoxelIndex south() const;
		VoxelIndex west() const;
		VoxelIndex above() const;
		VoxelIndex below() const;
	};
}