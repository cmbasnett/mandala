#pragma once

// std
#include <memory>
#include <vector>
#include <array>

// type
#include "voxel_chunk.hpp"
#include "voxel_data.hpp"

namespace naga
{
	struct VoxelMap
	{
		enum class CardinalDirection : u8
		{
			NORTH,
			EAST,
			SOUTH,
			WEST,
			ABOVE,
			BELOW,
		};

		typedef std::shared_ptr<VoxelChunk> ChunkType;

		VoxelMap();

		void set_data_source(const std::shared_ptr<VoxelDataSource>& data_source);
		void set_chunk_size(size_t chunk_size);
		void set_max_chunks(size_t max_chunks);

	private:
		std::shared_ptr<VoxelDataSource> data_source;
		std::map<size_t, ChunkType> chunks;	// TODO: tack on the currently loaded x/y/z index for the chunk
		// TODO: 3x vector
		std::vector<std::vector<std::vector<ChunkType>>> chunk_map;

		// TODO: make a 3d mapping of where we are in the currently loaded chunks that is conducive to shifting by an arbitrary amount along a given axis
		size_t max_chunks = 9;

		const ChunkType& get_chunk(vec3_u32 index) const;
		void update_chunk_buffers(ChunkType& chunk) const;
		void load_chunk(ChunkType& chunk) const;
		void render_chunk(ChunkType& chunk) const;

		// TODO: chunk loading could be complicated, we'll have to load the map as we go, and 
		// TODO: maximum loaded chunks exist in the map, the voxel world is some sort of streaming interface that we can call upon in an asynchronous way
		// TODO: moving the pointers around in the list shouldn't be terribly difficult!


	};
}