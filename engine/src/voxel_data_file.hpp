#pragma once

// std
#include <fstream>

// naga
#include "voxel_data.hpp"

namespace naga
{
	struct VoxelDataFileStream : VoxelDataSource
	{
		VoxelDataFileStream(const std::string& filename);

		virtual std::future<VoxelDataSource::LoadResponse> load_data(VoxelDataSource::LoadRequest request) override;
		virtual std::future<VoxelDataSource::SaveResponse> save_data(VoxelDataSource::SaveRequest request) override;

	private:
		std::mutex stream_mutex;
		std::fstream stream;
	};
}
