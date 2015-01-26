//mandala
#include "voxel_map.hpp"
#include "voxel_type.hpp"

namespace mandala
{
	voxel_map_t::chunk_t::chunk_t()
	{
		vertex_buffer = std::make_shared<vertex_buffer_type>();
		index_buffer = std::make_shared<index_buffer_type>();
	}

	void voxel_map_t::chunk_t::update_visibility()
	{
		for (size_t x = 0; x < chunk_size; ++x)
		{
			for (size_t y = 0; y < chunk_size; ++y)
			{
				for (size_t z = 0; z < chunk_size; ++z)
				{
					auto& voxel = voxels[x][y][z];

					voxel.visibility_flags = voxel_visibility_flag_none;

					if (x > 0)
					{
						if (voxels[x - 1][y][z].type->is_visible)
						{
							voxel.visibility_flags |= voxel_visibility_flag_right;
						}
					}
					else if (x == chunk_size - 1)
					{
						if (voxels[x + 1][y][z].type->is_visible)
						{
							voxel.visibility_flags |= voxel_visibility_flag_left;
						}
					}

					if (y > 0)
					{
						if (voxels[x][y - 1][z].type->is_visible)
						{
							voxel.visibility_flags |= voxel_visibility_flag_down;
						}
					}
					else if (y == chunk_size - 1)
					{
						if (voxels[x][y + 1][z].type->is_visible)
						{
							voxel.visibility_flags |= voxel_visibility_flag_up;
						}
					}

					if (z > 0)
					{
						if (voxels[x][y][z - 1].type->is_visible)
						{
							voxel.visibility_flags |= voxel_visibility_flag_back;
						}
					}
					else if (z == chunk_size - 1)
					{
						if (voxels[x][y][z + 1].type->is_visible)
						{
							voxel.visibility_flags |= voxel_visibility_flag_front;
						}
					}
				}
			}
		}
	}

	void voxel_map_t::chunk_t::update_buffers()
	{
		std::vector<vertex_buffer_type::vertex_type> vertices;

		vertex_buffer->data(vertices, gpu_t::buffer_usage_e::dynamic_draw);
	}
}
