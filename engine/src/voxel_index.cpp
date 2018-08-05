// naga
#include "voxel_index.hpp"
#include "voxel_chunk.hpp"

namespace naga
{
	VoxelIndex VoxelIndex::north() const
	{
		auto result = *this;
		result.index.y += 1;
		if (result.index.y >= VoxelChunk::SIZE)
		{
			result.chunk_index.y += 1;
			result.index.y = 0;
		}
		return result;
	}

	VoxelIndex VoxelIndex::east() const
	{
		auto result = *this;
		result.index.x += 1;
		if (result.index.x >= VoxelChunk::SIZE)
		{
			result.chunk_index.x += 1;
			result.index.x = 0;
		}
		return result;
	}

	VoxelIndex VoxelIndex::south() const
	{
		auto result = *this;
		result.index.y -= 1;
		if (result.index.y <= 0)
		{
			result.chunk_index.y -= 1;
			result.index.y = VoxelChunk::SIZE - 1;
		}
		return result;
	}

	VoxelIndex VoxelIndex::west() const
	{
		auto result = *this;
		result.index.x -= 1;
		if (result.index.x <= 0)
		{
			result.chunk_index.x -= 1;
			result.index.x = VoxelChunk::SIZE - 1;
		}
		return result;
	}

	VoxelIndex VoxelIndex::above() const
	{
		auto result = *this;
		result.index.z += 1;
		if (result.index.z >= VoxelChunk::SIZE)
		{
			result.chunk_index.z += 1;
			result.index.z = 0;
		}
		return result;
	}

	VoxelIndex VoxelIndex::below() const
	{
		auto result = *this;
		result.index.z -= 1;
		if (result.index.z <= 0)
		{
			result.chunk_index.z -= 1;
			result.index.z = VoxelChunk::SIZE - 1;
		}
		return result;
	}
}