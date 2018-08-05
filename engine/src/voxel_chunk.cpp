// naga
#include "voxel_chunk.hpp"
#include "gpu_buffer_mgr.hpp"

namespace naga
{
	VoxelChunk::VoxelChunk()
	{
		vertex_buffer = gpu_buffers.make<VertexBufferType>().lock();
		index_buffer = gpu_buffers.make<IndexBufferType>().lock();
	}

	void VoxelChunk::update_buffers()
	{
		// TODO: buffers are going to be highly dependent on what's adjacent, for now we skip this
	}
}