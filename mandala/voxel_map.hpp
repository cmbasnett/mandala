#pragma once

//std
#include <array>
#include <list>

//mandala
#include "gpu_vertices.hpp"
#include "voxel.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

namespace mandala
{
	struct voxel_map_t
	{
		static const size_t chunk_size = 8;

		struct chunk_t
		{
			chunk_t();	//TODO: construct with a stream

			typedef std::array<std::array<std::array<voxel_t, chunk_size>, chunk_size>, chunk_size> voxels_type;
			typedef vertex_buffer_t<voxel_vertex_t> vertex_buffer_type;
			typedef index_buffer_t<uint16_t> index_buffer_type;
			
			voxel_visibility_flag_type visibility_flags = voxel_visibility_flag_none;
			voxels_type voxels;
			std::shared_ptr<vertex_buffer_type> vertex_buffer;
			std::shared_ptr<index_buffer_type> index_buffer;

			void update_visibility();
			void update_buffers();
		};

		std::list<std::list<std::list<std::unique_ptr<chunk_t>>>> chunks;

		voxel_map_t() = default;
	};
}
