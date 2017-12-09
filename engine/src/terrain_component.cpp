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
#include "resource_manager.hpp"
#include "collision.hpp"
#include "debug_renderer.hpp"
#include "quadtree.hpp"
#include "game_object.hpp"

//bullet
#include <BulletCollision\CollisionShapes\btHeightfieldTerrainShape.h>

namespace naga
{
    const char* TerrainComponent::component_name = "TerrainComponent";

	void TerrainComponent::set_heightmap(const boost::shared_ptr<Image>& image)
    {
        heightmap = boost::make_shared<naga::Heightmap>(image);
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

        const ChunkIndexType chunk_x_count = width / CHUNK_SIZE;
		const ChunkIndexType chunk_z_count = depth / CHUNK_SIZE;

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

		std::vector<VertexType> vertices;

        // vertex buffer
		for (ChunkIndexType chunk_z = 0; chunk_z < chunk_z_count; ++chunk_z)
        {
			for (ChunkIndexType chunk_x = 0; chunk_x < chunk_x_count; ++chunk_x)
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

						vertices.push_back(VertexType(vertex, vec4(y, y, y, 1)));
                    }
                }
            }
        }

        vertex_buffer = gpu_buffers.make<VertexBufferType>().lock();
        vertex_buffer->data(vertices, Gpu::BufferUsage::DYNAMIC_DRAW);

        // index buffer
        std::vector<IndexType> indices;
        indices.reserve(chunk_count * INDICES_PER_CHUNK);

		for (ChunkIndexType chunk_z = 0; chunk_z < chunk_z_count; ++chunk_z)
        {
			for (ChunkIndexType chunk_x = 0; chunk_x < chunk_x_count; ++chunk_x)
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

                        const IndexType patch_indices[4] = {
                            patch_index_start + 0,
                            patch_index_start + 1,
                            patch_index_start + CHUNK_SIZE + 1,
                            patch_index_start + CHUNK_SIZE + 2
                        };

						std::vector<IndexType> local_patch_indices;

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
						triangles.push_back(Triangle3(this->vertices[patch_indices[local_patch_indices[0]]], this->vertices[patch_indices[local_patch_indices[1]]], this->vertices[patch_indices[local_patch_indices[2]]]));
						triangles.push_back(Triangle3(this->vertices[patch_indices[local_patch_indices[3]]], this->vertices[patch_indices[local_patch_indices[4]]], this->vertices[patch_indices[local_patch_indices[5]]]));

                        for (auto local_patch_index : local_patch_indices)
                        {
                            indices.push_back(patch_indices[local_patch_index]);
                        }
                    }
                }
            }
        }

        index_buffer = gpu_buffers.make<IndexBufferType>().lock();
        index_buffer->data(indices, Gpu::BufferUsage::STATIC_DRAW);

		btHeightfieldTerrainShape* heightfield_terrain_shape = new btHeightfieldTerrainShape(
            static_cast<int>(image->get_size().x),
            static_cast<int>(image->get_size().y),
            heightmap->get_data(),
            0.0f,
            0.0f,
            scale.y,
            1,
            PHY_FLOAT,
            false);
		heightfield_terrain_shape->setLocalScaling(btVector3(1.0f, scale.y, 1.0f));
		auto rigid_body = get_owner()->add_component<RigidBodyComponent>();
		rigid_body->set_collision_shape(heightfield_terrain_shape);

        texture = resources.get<Texture>("white.png");		
		quadtree = boost::make_shared<QuadTree>(static_cast<f32>(width), scale.y, static_cast<f32>(CHUNK_SIZE));
    }

	void TerrainComponent::on_render(CameraParameters& camera_parameters)
	{
        gpu.buffers.push(Gpu::BufferTarget::ARRAY, vertex_buffer);
        gpu.buffers.push(Gpu::BufferTarget::ELEMENT_ARRAY, index_buffer);

        const auto gpu_program = gpu_programs.get<basic_gpu_program>();

        gpu.programs.push(gpu_program);

		auto culling_state = Gpu::CullingStateManager::CullingState();
		culling_state.is_enabled = true;
		culling_state.mode = Gpu::CullingMode::BACK;
		culling_state.front_face = Gpu::CullingFrontFace::CW;
		gpu.culling.push_state(culling_state);

		gpu.set_uniform("world_matrix", glm::scale(scale));
		gpu.set_uniform("view_projection_matrix", camera_parameters.projection_matrix * camera_parameters.view_matrix);
        gpu.set_uniform("diffuse_texture", 0);
        gpu.set_uniform("color", vec4(1.0f));
        gpu.set_uniform("texture_scale", 0.03125f);

        gpu.textures.bind(0, texture);

        gpu.draw_elements(Gpu::PrimitiveType::TRIANGLES, INDICES_PER_CHUNK * chunk_count, IndexBufferType::DATA_TYPE, 0);

#ifdef DEBUG
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//gpu.set_uniform("color", vec4(1.0f));
		//gpu.draw_elements(Gpu::PrimitiveType::TRIANGLES, INDICES_PER_CHUNK * chunk_count, IndexBufferType::DATA_TYPE, 0);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

		gpu.culling.pop_state();

        gpu.textures.unbind(0);

        gpu.programs.pop();

        gpu.buffers.pop(Gpu::BufferTarget::ELEMENT_ARRAY);
		gpu.buffers.pop(Gpu::BufferTarget::ARRAY);

#ifdef DEBUG
		std::function<void(const QuadTree::Node*)> render_quadtree_node = [&](const QuadTree::Node* node) {
			if (node == nullptr)
			{
				return;
			}

			auto is_traced = std::find(traced_nodes.begin(), traced_nodes.end(), node) != traced_nodes.end();
			auto color = is_traced ? vec4(1) : vec4(1, 0, 0, 1);

			if (node->is_leaf() && is_traced)
			{
				render_aabb(mat4(), camera_parameters.projection_matrix * camera_parameters.view_matrix, node->get_bounds(), color);
			}

			for (auto& child : node->get_children())
			{
				render_quadtree_node(child);
			}
		};
		render_quadtree_node(quadtree->get_root());
#endif
	}

    f32 TerrainComponent::get_height(const vec2& location) const
	{
        return heightmap->get_height(location);
	}

	vec3 TerrainComponent::trace(const Line3& ray)
	{
		traced_nodes.clear();

		auto nodes = quadtree->trace(ray);

		std::copy(nodes.begin(), nodes.end(), std::back_inserter(traced_nodes));

		vec3 hit_location;

		const auto chunks_per_strip = width / CHUNK_SIZE;

		for (const auto& node : nodes)
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
				const vec3& v1 = triangle[0];
				const vec3& v2 = triangle[1];
				const vec3& v3 = triangle[2];
				f32 min_x = std::numeric_limits<f32>::max();
				vec3 barycentric_position;

				if (intersectLineTriangle(ray.start, ray.direction(), v1, v2, v3, barycentric_position))
				{
					if (glm::dot(triangle.normal(), ray.direction()) > 0.0) {
						if (barycentric_position.x < min_x) {
							min_x = barycentric_position.x;
							hit_location = ray.start + (ray.direction() * barycentric_position.x);
						}
						return hit_location;
					}
				}
			}
		}

        return vec3();
    }

	void TerrainComponent::update_chunks(const details::Rectangle<u64>& rectangle)
    {
        //TODO: get all affected chunks in this rectangle
    }

	void TerrainComponent::set_scale(const vec3& scale)
	{
		this->scale = scale;
	}
}