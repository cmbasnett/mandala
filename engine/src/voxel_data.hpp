#pragma once

// std
#include <future>
#include <map>

// boost
#include <boost/optional.hpp>

// naga
#include "types.hpp"
#include "voxel.hpp"
#include "voxel_chunk.hpp"

namespace naga
{
	// TODO: for performance reasons, we shouldn't try and initialize a massive world all at once
	// we should store only large data blocks as they are generated. we can then quary for the data
	// if it doesn't yet exist from the underlying data stream (a memory mapped file somewhere?)
	// OR EVEN ONLINE :O (generic interface!) use promises to resolve queries etc.

	struct VoxelDataSource
	{
		struct LoadRequest
		{
			std::vector<size_t> chunk_ids;
		};

		enum class LoadError
		{
			EMPTY_REQUEST,
			BAD_CHUNK_ID,
		};

		struct LoadResponse
		{
			LoadRequest request;
			boost::optional<LoadError> error;
			std::map<size_t, VoxelChunk> chunks;
		};

		struct SaveResponse
		{
		};

		struct SaveRequest
		{
		};

		virtual std::future<LoadResponse> load_data(LoadRequest request) = 0;
		virtual std::future<SaveResponse> save_data(SaveRequest request) = 0;
	};
}
