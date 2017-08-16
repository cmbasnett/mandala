//glm
#include <glm/ext.hpp>

//naga
#include "terrain_component.hpp"
#include "gpu_buffer_mgr.hpp"
#include "camera_params.hpp"
#include "gpu_program_mgr.hpp"
#include "opengl.hpp"
#include "image.hpp"
#include "rigid_body_component.hpp"
#include "texture.hpp"
#include "resource_mgr.hpp"
#include "collision.hpp"
#include "line_renderer.hpp"
#include "quadtree.hpp"

//bullet
#include <BulletCollision\CollisionShapes\btHeightfieldTerrainShape.h>

namespace naga
{
    const char* terrain_component::component_name = "TerrainComponent";

    void terrain_component::set_heightmap(const boost::shared_ptr<image>& image)
    {
        heightmap = boost::make_shared<naga::heightmap>(image);

		width = static_cast<i32>(image->get_width());
		depth = static_cast<i32>(image->get_height());

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

		width -= 1;
		depth -= 1;

        const chunk_index_type chunk_x_count = width / CHUNK_SIZE;
        const chunk_index_type chunk_z_count = depth / CHUNK_SIZE;

        if (chunk_x_count % CHUNK_SIZE != 0)
        {
            std::ostringstream oss;
            oss << "width must be multiple of " << CHUNK_SIZE;
            throw std::invalid_argument(oss.str());
        }

        if (chunk_z_count % CHUNK_SIZE != 0)
        {
            std::ostringstream oss;
            oss << "height must be multiple of " << CHUNK_SIZE;
            throw std::invalid_argument(oss.str());
        }

        chunk_count = chunk_x_count * chunk_z_count;

        vertices.reserve(chunk_count * VERTICES_PER_CHUNK);

		auto half_width = static_cast<i32>(width / 2);
		auto half_depth = static_cast<i32>(depth / 2);

		std::vector<vertex_type> vertices;

        // vertex buffer
        for (chunk_index_type chunk_z = 0; chunk_z < chunk_z_count; ++chunk_z)
        {
            for (chunk_index_type chunk_x = 0; chunk_x < chunk_x_count; ++chunk_x)
            {
                for (auto patch_z = 0; patch_z <= CHUNK_SIZE; ++patch_z)
                {
                    for (auto patch_x = 0; patch_x <= CHUNK_SIZE; ++patch_x)
                    {
						auto x = (chunk_x * CHUNK_SIZE) + patch_x;
						auto z = (chunk_z * CHUNK_SIZE) + patch_z;
                        auto y = heightmap->get_data(x, z);

						auto vertex = vec3(static_cast<i32>(x) - half_width, y, static_cast<i32>(z) - half_depth) * scale;

						this->vertices.push_back(vertex);

						vertices.push_back(vertex_type(vertex, vec4(y, y, y, 1)));
                    }
                }
            }
        }

        vertex_buffer = gpu_buffers.make<vertex_buffer_type>().lock();
        vertex_buffer->data(vertices, gpu_t::buffer_usage::DYNAMIC_DRAW);

        // index buffer
        std::vector<index_type> indices;
        indices.reserve(chunk_count * INDICES_PER_CHUNK);

        for (chunk_index_type chunk_z = 0; chunk_z < chunk_z_count; ++chunk_z)
        {
            for (chunk_index_type chunk_x = 0; chunk_x < chunk_x_count; ++chunk_x)
            {
                const auto chunk_index_start = (chunk_x + (chunk_z * chunk_x_count)) * VERTICES_PER_CHUNK;

                for (auto patch_z = 0; patch_z < CHUNK_SIZE; ++patch_z)
                {
                    const auto strip_index_start = chunk_index_start + ((CHUNK_SIZE + 1) * patch_z);

                    for (auto patch_x = 0; patch_x < CHUNK_SIZE; ++patch_x)
                    {
                        // z
                        // ^ 2---3
                        // | |\ /|
                        // | | x |
                        // | |/ \|
                        // | 0---1
                        // +----> x
                        const auto patch_index_start = strip_index_start + patch_x;

                        const index_type patch_indices[4] = {
                            patch_index_start + 0,
                            patch_index_start + 1,
                            patch_index_start + CHUNK_SIZE + 1,
                            patch_index_start + CHUNK_SIZE + 2
                        };

                        std::vector<index_type> local_patch_indices;

                        //TODO: figure out local patch indices based on patch
                        if ((patch_x % 2 == 0) ^ (patch_z % 2 == 0))
                        {
                            local_patch_indices = { 0, 1, 3, 0, 3, 2 };
                        }
						else
						{
							local_patch_indices = { 0, 1, 2, 1, 3, 2 };
						}
						
						// add the triangles
						// TODO: do a better job of this!
						triangles.push_back(triangle3(this->vertices[patch_indices[local_patch_indices[0]]], this->vertices[patch_indices[local_patch_indices[1]]], this->vertices[patch_indices[local_patch_indices[2]]]));
						triangles.push_back(triangle3(this->vertices[patch_indices[local_patch_indices[3]]], this->vertices[patch_indices[local_patch_indices[4]]], this->vertices[patch_indices[local_patch_indices[5]]]));

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

  //      btHeightfieldTerrainShape* heightfield_terrain_shape = new btHeightfieldTerrainShape(
  //          image->get_size().x,
  //          image->get_size().y,
  //          heightmap->get_data(),
  //          scale.x,
  //          -scale.y / 2,
  //          scale.z / 2,
  //          1,
  //          PHY_FLOAT,
  //          false);

  //      auto rigid_body = boost::make_shared<rigid_body_component>();
		//rigid_body->set_collision_shape(heightfield_terrain_shape);

        texture = resources.get<naga::texture>("white.png");		
		quadtree = boost::make_shared<naga::quadtree>(static_cast<f32>(width), scale.y, static_cast<f32>(CHUNK_SIZE));
    }

    void terrain_component::on_render(camera_params& camera_params)
	{
        gpu.buffers.push(gpu_t::buffer_target::ARRAY, vertex_buffer);
        gpu.buffers.push(gpu_t::buffer_target::ELEMENT_ARRAY, index_buffer);

        const auto gpu_program = gpu_programs.get<basic_gpu_program>();

        gpu.programs.push(gpu_program);

		gpu.set_uniform("world_matrix", glm::scale(scale));
        gpu.set_uniform("view_projection_matrix", camera_params.projection_matrix * camera_params.view_matrix);
        gpu.set_uniform("diffuse_texture", 0);
        gpu.set_uniform("color", vec4(1.0f));
        gpu.set_uniform("texture_scale", 0.03125f);

        gpu.textures.bind(0, texture);

        gpu.draw_elements(gpu_t::primitive_type::TRIANGLES, INDICES_PER_CHUNK * chunk_count, index_buffer_type::DATA_TYPE, 0);

        gpu.textures.unbind(0);

        gpu.programs.pop();

        gpu.buffers.pop(gpu_t::buffer_target::ELEMENT_ARRAY);
		gpu.buffers.pop(gpu_t::buffer_target::ARRAY);

#ifdef DEBUG
		std::function<void(const quadtree::node*)> render_quadtree_node = [&](const quadtree::node* node) {
			if (node == nullptr)
			{
				return;
			}

			auto is_traced = std::find(traced_nodes.begin(), traced_nodes.end(), node) != traced_nodes.end();
			auto color = is_traced ? vec4(1) : vec4(1, 0, 0, 1);

			if (node->is_leaf() && is_traced)
			{
				render_aabb(mat4(), camera_params.projection_matrix * camera_params.view_matrix, node->get_bounds(), color);
			}

			for (auto& child : node->get_children())
			{
				render_quadtree_node(child);
			}
		};
		render_quadtree_node(quadtree->get_root());
#endif
	}

    f32 terrain_component::get_height(const vec2& location) const
	{
        return heightmap->get_height(location);
	}

    vec3 terrain_component::trace(const line3& ray)
	{
		traced_nodes.clear();

		auto nodes = quadtree->trace(ray);

		std::copy(nodes.begin(), nodes.end(), std::back_inserter(traced_nodes));

		vec3 hit_location;

		std::cout << nodes.size() << std::endl;

		const auto chunks_per_strip = width / CHUNK_SIZE;

		for (auto& node : nodes)
		{
			// TODO: get chunk indices index from node!
			const auto& bounds = node->get_bounds() + vec3(quadtree->get_bounds().width() / 2, 0, quadtree->get_bounds().depth() / 2);
			const auto x = static_cast<i32>(bounds.min.x / scale.x / CHUNK_SIZE);
			const auto z = static_cast<i32>(bounds.min.z / scale.z / CHUNK_SIZE);
			const auto chunk_index = (z * chunks_per_strip + x);
			const auto triangle_start_index = chunk_index * TRIANGLES_PER_CHUNK;

			for (i32 i = triangle_start_index; i < triangle_start_index + TRIANGLES_PER_CHUNK; ++i)
			{
				auto triangle = triangles[i];
				vec3 v1 = triangle[0];
				vec3 v2 = triangle[1];
				vec3 v3 = triangle[2];

				if (intersectLineTriangle(ray.start, ray.direction(), v1, v2, v3, hit_location))
				{
					hit_location = ray.start + (ray.direction() * hit_location.x);
					// TODO: convert from bary to world

					return hit_location;
				}
			}
		}

        return vec3();
    }

    void terrain_component::update_chunks(const rectangle_u64& rectangle)
    {
        //TODO: get all affected chunks in this rectangle
    }

	void terrain_component::set_scale(const vec3& scale)
	{
		this->scale = scale;
	}
}