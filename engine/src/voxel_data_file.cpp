// naga
#include "voxel_data_file.hpp"

namespace naga
{
	VoxelDataFileStream::VoxelDataFileStream(const std::string& filename)
	{
		stream = std::fstream(filename.c_str(), std::ios::binary);
	}

	std::future<VoxelDataSource::LoadResponse> VoxelDataFileStream::load_data(VoxelDataSource::LoadRequest request)
	{
		LoadResponse response;
		std::promise<LoadResponse> promise;

		response.request = request;	// TODO: deep copy of vector, i hope!

		if (request.chunk_ids.empty())
		{
			response.error = VoxelDataSource::LoadError::EMPTY_REQUEST;
			promise.set_value(response);
		}

		// TODO: just return random voxels for now!
		for (auto chunk_id : request.chunk_ids)
		{
			VoxelChunk::VoxelsType voxels;

			for (auto x : voxels)
			{
				for (auto y : x)
				{
					for (auto z : y)
					{
						z.type = rand() % 2;
					}
				}
			}

			response.chunks.emplace(std::make_pair(chunk_id, voxels));
		}

		return promise.get_future();
	}
}
