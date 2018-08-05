#pragma once

// std
#include <array>

// naga
#include "aabb.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "basic_gpu_program.hpp"
#include "voxel_index.hpp"

namespace naga
{
	struct Voxel;
	struct VoxelMap;

	template<typename PointerType>
	struct VoxelsIterator
	{

	};

	struct VoxelChunk
	{
		static const size_t SIZE = 8;

		typedef std::array<std::array<std::array<Voxel, SIZE>, SIZE>, SIZE> VoxelsType;
		typedef vec3_i32 VoxelsIndexType;


		enum class State	// TODO: when we figure out the exact stages, we can sort this out
		{
			UNINITIALIZED,	// Chunk has been allocated but look-up table data has not been initialized.
			QUEUED,			// Chunk is queued for loading.
			LOADING,		// Chunk is loading.
			LOADED,			// Chunk is loaded.
		};

		size_t id;
		VoxelIndex::IndexType index;
		naga::details::AABB2<i32> aabb;
		State state;
		VoxelsType voxels;

		void update_buffers();

		const VoxelType& get_voxel(VoxelsIndexType index);

		typedef VertexBuffer<basic_gpu_program::VertexType> VertexBufferType;
		typedef IndexBuffer<u16> IndexBufferType;

	private:
		boost::shared_ptr<VertexBufferType> vertex_buffer;
		boost::shared_ptr<IndexBufferType> index_buffer;
	};
}
