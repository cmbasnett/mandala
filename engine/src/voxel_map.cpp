// std
#include <future>

// naga
#include "voxel_map.hpp"
#include "voxel_data.hpp"
#include "voxel_index.hpp"

namespace naga
{
	VoxelMap::VoxelMap() :
		data_source(nullptr)
	{
		// TODO: verify correctness
		for (size_t x = 0; x < max_chunks; ++x)
		{
			std::list<std::list<ChunkType>> ylist;

			for (size_t y = 0; y < max_chunks; ++y)
			{
				std::list<ChunkType> zlist;

				for (size_t z = 0; z < max_chunks; ++z)
				{
					zlist.push_back(std::make_shared<VoxelChunk>());
				}

				ylist.push_back(zlist);
			}

			chunk_map.push_back(ylist);
		}
	}

	void VoxelMap::set_max_chunks(size_t max_chunks)
	{
		if (this->max_chunks == max_chunks)
		{
			return;
		}
		else if (max_chunks > this->max_chunks)
		{
			// TODO: expand!
		}
		else
		{
			// TODO: contract!
		}

		this->max_chunks = max_chunks;
	}

	void VoxelMap::set_data_source(const std::shared_ptr<VoxelDataSource>& data_source)
	{
		if (this->data_source == data_source)
		{
			return;
		}

		this->data_source = data_source;

		chunks.clear();
	}

	void VoxelMap::load_chunk(VoxelMap::ChunkType& chunk) const
	{
#if DEBUG
		if (data_source == nullptr)
		{
			throw std::exception("Data source is missing.");
		}
#endif

		VoxelDataSource::LoadRequest request;
		request.chunk_ids.push_back(chunk->id);

		auto promise = data_source->load_data(request);

		if (promise.valid())
		{
			promise.get();
		}
	}

	const VoxelMap::ChunkType& VoxelMap::get_chunk(VoxelIndex::IndexType index) const
	{
		return chunk_map[index.x][index.y][index.z];
	}

	void VoxelMap::update_chunk_buffers(ChunkType& chunk) const
	{
		typedef VoxelChunk::VertexBufferType::VertexType VertexType;

		std::vector<VertexType> vertices;

		VoxelIndex voxel_index;
		voxel_index.chunk_index = chunk->index;

		for (auto x = 0; x < VoxelChunk::SIZE; ++x)
		{
			for (auto y = 0; y < VoxelChunk::SIZE; ++y)
			{
				for (auto z = 0; z < VoxelChunk::SIZE; ++z)
				{
					const auto& voxel = chunk->voxels[x][y][z];

					VoxelIndex voxel_index;

					if (voxel.type == 1)
					{
					}

					// neswtd
					// +y, +x, -y, -x, +z, -z
					if (voxel.type == 1)
					{
					}
				}
			}
		}
	}
}