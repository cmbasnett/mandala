#pragma once

#include <memory>

#include "types.hpp"

namespace naga
{
	struct VoxelType;

	struct Voxel
	{
		//std::weak_ptr<VoxelType> type;
		u8 type;
		void* user_data;	// TODO: pointing to perhaps some sort of python_ptr?
	};
}