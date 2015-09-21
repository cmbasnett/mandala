#include "terrain.hpp"
#include "gpu_buffer_mgr.hpp"

namespace mandala
{
    terrain_t::terrain_t(size_type width, size_type depth) :
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

        if (width > max_size)
        {
            std::ostringstream oss;
            oss << "width cannot exceed " << max_size;
            throw std::invalid_argument(oss.str());
        }

        if (depth > max_size)
        {
            std::ostringstream oss;
            oss << "width cannot exceed " << max_size;
            throw std::invalid_argument(oss.str());
        }

        if (width % chunk_size != 0)
        {
            std::ostringstream oss;
            oss << "width must be multiple of " << chunk_size;
            throw std::invalid_argument(oss.str());
        }

        if (depth % chunk_size != 0)
        {
            std::ostringstream oss;
            oss << "height must be multiple of " << chunk_size;
            throw std::invalid_argument(oss.str());
        }

        const chunk_index_type chunk_x_count = width / chunk_size;
        const chunk_index_type chunk_z_count = depth / chunk_size;
        const auto chunk_count = chunk_x_count * chunk_z_count;

        std::vector<vertex_type> vertices;
        vertices.reserve(chunk_count * vertices_per_chunk);

        // build vertex buffer
        for (chunk_index_type chunk_z = 0; chunk_z < chunk_z_count; ++chunk_z)
        {
            for (chunk_index_type chunk_x = 0; chunk_x < chunk_x_count; ++chunk_x)
            {
                for (auto patch_z = 0; patch_z <= chunk_size; ++patch_z)
                {
                    for (auto patch_x = 0; patch_x <= chunk_size; ++patch_x)
                    {
                        vertices.push_back(vertex_type(glm::vec3((chunk_x * chunk_size) + patch_x, 0, (chunk_z * chunk_size) + patch_z), glm::vec4(1)));
                    }

                    if (patch_z < chunk_size)
                    {
                        for (auto patch_x = 0; patch_x < chunk_size; ++patch_x)
                        {
                            vertices.push_back(vertex_type(glm::vec3((chunk_x * chunk_size) + patch_x + 0.5f, 0, (chunk_z * chunk_size) + patch_z + 0.5f), glm::vec4(1)));
                        }
                    }
                }
            }
        }

        vertex_buffer->data(vertices, gpu_t::buffer_usage_e::DYNAMIC_DRAW);

        // index buffer
        std::vector<index_type> indices;
        indices.reserve(chunk_count * indices_per_chunk);

        for (chunk_index_type chunk_x = 0; chunk_x < chunk_x_count; ++chunk_x)
        {
            for (chunk_index_type chunk_z = 0; chunk_z < chunk_z_count; ++chunk_z)
            {
                const auto chunk_index_start = (chunk_z * chunk_x_count) * indices_per_chunk;

                for (auto patch_z = 0; patch_z <= chunk_size; ++patch_z)
                {
                    const auto strip_index_start = chunk_index_start + (indices_per_strip * patch_z);

                    for (auto patch_x = 0; patch_x < chunk_size; ++patch_x)
                    {
                        // z
                        // ^ 3---4
                        // | | 2 |
                        // | 0---1
                        // +---- > x
                        static const auto local_patch_indices = { 2, 0, 1, 2, 1, 4, 2, 4, 3, 2, 3, 0 };
                        const auto patch_index_start = strip_index_start + (indices_per_patch * patch_x);

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
        index_buffer->data(indices, gpu_t::buffer_usage_e::STATIC_DRAW);
    }
}