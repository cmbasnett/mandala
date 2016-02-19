#include "terrain.hpp"
#include "gpu_buffer_mgr.hpp"

namespace naga
{
    terrain::terrain(size_type width, size_type depth) :
        heightmap(width, depth),
        quadtree(std::max(width, depth))
    {
        if (width == 0)
        {
            throw std::invalid_argument("width cannot be 0");
        }

        if (depth == 0)
        {
            throw std::invalid_argument("height cannot be 0");
        }

        if (width > MAX_SIZE)
        {
            std::ostringstream oss;
            oss << "width cannot exceed " << MAX_SIZE;
            throw std::invalid_argument(oss.str());
        }

        if (depth > MAX_SIZE)
        {
            std::ostringstream oss;
            oss << "width cannot exceed " << MAX_SIZE;
            throw std::invalid_argument(oss.str());
        }

        if (width % CHUNK_SIZE != 0)
        {
            std::ostringstream oss;
            oss << "width must be multiple of " << CHUNK_SIZE;
            throw std::invalid_argument(oss.str());
        }

        if (depth % CHUNK_SIZE != 0)
        {
            std::ostringstream oss;
            oss << "height must be multiple of " << CHUNK_SIZE;
            throw std::invalid_argument(oss.str());
        }

        const chunk_index_type chunk_x_count = width / CHUNK_SIZE;
        const chunk_index_type chunk_z_count = depth / CHUNK_SIZE;
        const auto chunk_count = chunk_x_count * chunk_z_count;

        std::vector<vertex_type> vertices;
        vertices.reserve(chunk_count * VERTICES_PER_CHUNK);

        // build vertex buffer
        for (chunk_index_type chunk_z = 0; chunk_z < chunk_z_count; ++chunk_z)
        {
            for (chunk_index_type chunk_x = 0; chunk_x < chunk_x_count; ++chunk_x)
            {
                for (auto patch_z = 0; patch_z <= CHUNK_SIZE; ++patch_z)
                {
                    for (auto patch_x = 0; patch_x <= CHUNK_SIZE; ++patch_x)
                    {
						vertices.push_back(vertex_type(glm::vec3((chunk_x * CHUNK_SIZE) + patch_x, 0, (chunk_z * CHUNK_SIZE) + patch_z), glm::vec4(1)));
                    }

                    if (patch_z < CHUNK_SIZE)
                    {
						for (auto patch_x = 0; patch_x < CHUNK_SIZE; ++patch_x)
                        {
							vertices.push_back(vertex_type(glm::vec3((chunk_x * CHUNK_SIZE) + patch_x + 0.5f, 0, (chunk_z * CHUNK_SIZE) + patch_z + 0.5f), glm::vec4(1)));
                        }
                    }
                }
            }
        }

        vertex_buffer->data(vertices, gpu_t::buffer_usage::DYNAMIC_DRAW);

        // index buffer
        std::vector<index_type> indices;
        indices.reserve(chunk_count * INDICES_PER_CHUNK);

        for (chunk_index_type chunk_x = 0; chunk_x < chunk_x_count; ++chunk_x)
        {
            for (chunk_index_type chunk_z = 0; chunk_z < chunk_z_count; ++chunk_z)
            {
                const auto chunk_index_start = (chunk_z * chunk_x_count) * INDICES_PER_CHUNK;

				for (auto patch_z = 0; patch_z <= CHUNK_SIZE; ++patch_z)
                {
                    const auto strip_index_start = chunk_index_start + (INDICES_PER_STRIP * patch_z);

                    for (auto patch_x = 0; patch_x < CHUNK_SIZE; ++patch_x)
                    {
                        // z
                        // ^ 3---4
                        // | | 2 |
                        // | 0---1
                        // +---- > x
                        static const auto local_patch_indices = { 2, 0, 1, 2, 1, 4, 2, 4, 3, 2, 3, 0 };
                        const auto patch_index_start = strip_index_start + (INDICES_PER_PATCH * patch_x);

                        const index_type patch_indices[5] = {
                            patch_index_start + 0,
                            patch_index_start + 1,
                            patch_index_start + chunk_x_count,
                            patch_index_start + (chunk_x_count * 2),
                            patch_index_start + (chunk_x_count * 2) + 1
                        };

                        for (auto local_patch_index : local_patch_indices)
                        {
                            indices.push_back(patch_indices[local_patch_index]);
                        }
                    }
                }
            }
        }

        index_buffer = gpu_buffers.make<index_buffer_type>().lock();
        index_buffer->data(indices, gpu_t::buffer_usage::STATIC_DRAW);
	}

	void terrain::render(const camera& camera) const
	{
	}

	f32 terrain::get_height(const vec2& location) const
	{
		std::function<bool(const boost::shared_ptr<quadtree::node>)> traverse_quadtree = [&](const boost::shared_ptr<quadtree::node> node)
		{
			if (!contains(node->get_bounds(), location))
			{
				return false;
			}

			if (node->is_leaf())
			{

				return true;
			}
			else
			{
				for (const auto child : node->get_children())
				{
					if (traverse_quadtree(child))
					{
						return true;
					}
				}

				return false;
			}
		};
		
		f32 height = 0.0f;

		traverse_quadtree(quadtree.get_root(), height);

		return height;
	}

	vec3 terrain::trace(const line3& ray) const
	{
		return vec3();
	}
}