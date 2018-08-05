//naga
#include "types.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "gpu_program_mgr.hpp"
#include "basic_gpu_program.hpp"
#include "rectangle.hpp"
#include "line.hpp"
#include "sphere.hpp"
#include "texture.hpp"
#include "resource_manager.hpp"
#include "gpu_buffer_mgr.hpp"

namespace naga
{
	inline void render_line_loop(const mat4& world_matrix, const mat4& view_projection_matrix, const std::vector<vec3>& points, const vec4& color)
	{
		static const size_t MAX_LINES = 1024;
		static const size_t VERTEX_COUNT = MAX_LINES + 1;
		static const size_t INDEX_COUNT = MAX_LINES;

		assert(points.size() <= MAX_LINES);

		typedef VertexBuffer<basic_gpu_program::VertexType> VertexBufferType;
		typedef IndexBuffer<IndexType<INDEX_COUNT>::Type> IndexBufferType;

		static boost::weak_ptr<VertexBufferType> vertex_buffer;
		static boost::weak_ptr<IndexBufferType> index_buffer;

		static std::array<basic_gpu_program::VertexType, VERTEX_COUNT> vertices;

		if (vertex_buffer.expired())
		{
			vertex_buffer = gpu_buffers.make<VertexBufferType>();

			for (auto& vertex : vertices)
			{
				vertex.color = vec4(1);
			}
		}

		for (size_t i = 0; i < points.size(); ++i)
		{
			vertices[i].location = points[i];
		}

		vertex_buffer.lock()->data(vertices.data(), points.size(), Gpu::BufferUsage::STREAM_DRAW);

		if (index_buffer.expired())
		{
			std::array<IndexBufferType::IndexType, INDEX_COUNT> indices;

			for (size_t i = 0; i < INDEX_COUNT; ++i)
			{
				indices[i] = i;
			}

			index_buffer = gpu_buffers.make<IndexBufferType>();
			index_buffer.lock()->data(indices.data(), indices.size(), Gpu::BufferUsage::STATIC_DRAW);
		}

		gpu.buffers.push(Gpu::BufferTarget::ARRAY, vertex_buffer.lock());
		gpu.buffers.push(Gpu::BufferTarget::ELEMENT_ARRAY, index_buffer.lock());

		const auto gpu_program = gpu_programs.get<basic_gpu_program>();

		gpu.programs.push(gpu_program);

		// TODO: have this as some sort of constant somewhere!
		auto texture = resources.get<Texture>("white.png");
		gpu.textures.bind(0, texture);

		gpu.set_uniform("world_matrix", world_matrix);
		gpu.set_uniform("view_projection_matrix", view_projection_matrix);
		gpu.set_uniform("color", color);
		gpu.set_uniform("diffuse_texture", 0);

		gpu.draw_elements(Gpu::PrimitiveType::LINE_STRIP, points.size(), IndexBufferType::DATA_TYPE, 0);

		gpu.textures.unbind(0);

		gpu.programs.pop();

		gpu.buffers.pop(Gpu::BufferTarget::ARRAY);
		gpu.buffers.pop(Gpu::BufferTarget::ELEMENT_ARRAY);
	}

	template<typename T>
	void render_rectangle(const mat4& world_matrix, const mat4& view_projection_matrix, const details::Rectangle<T>& rectangle, const vec4& color, bool is_filled = false)
	{
		typedef VertexBuffer<basic_gpu_program::VertexType> VertexBufferType;
		typedef IndexBuffer<u8> IndexBufferType;

		static boost::weak_ptr<VertexBufferType> vertex_buffer;
		static boost::weak_ptr<IndexBufferType> index_buffer;

		if (vertex_buffer.expired())
		{
			vertex_buffer = gpu_buffers.make<VertexBufferType>();
			auto vertices = {
				basic_gpu_program::VertexType(vec3(0, 0, 0), vec4(1)),
				basic_gpu_program::VertexType(vec3(1, 0, 0), vec4(1)),
				basic_gpu_program::VertexType(vec3(1, 1, 0), vec4(1)),
				basic_gpu_program::VertexType(vec3(0, 1, 0), vec4(1))
			};
			vertex_buffer.lock()->data(vertices, Gpu::BufferUsage::STATIC_DRAW);
		}

		if (index_buffer.expired())
		{
			index_buffer = gpu_buffers.make<IndexBufferType>();
			index_buffer.lock()->data({ 0, 1, 2, 3 }, Gpu::BufferUsage::STATIC_DRAW);
		}

		gpu.buffers.push(Gpu::BufferTarget::ARRAY, vertex_buffer.lock());
		gpu.buffers.push(Gpu::BufferTarget::ELEMENT_ARRAY, index_buffer.lock());

		const auto gpu_program = gpu_programs.get<basic_gpu_program>();

		gpu.programs.push(gpu_program);

		// TODO: have this as some sort of constant somewhere!
		auto texture = resources.get<Texture>("white.png");
		gpu.textures.bind(0, texture);

		gpu.set_uniform("world_matrix", world_matrix * glm::translate(vec3(rectangle.x, rectangle.y, T(0))) * glm::scale(vec3(rectangle.width, rectangle.height, T(0))));
		gpu.set_uniform("view_projection_matrix", view_projection_matrix);
		gpu.set_uniform("color", color);

		gpu.draw_elements(is_filled ? Gpu::PrimitiveType::TRIANGLE_FAN : Gpu::PrimitiveType::LINE_LOOP, 4, IndexBufferType::DATA_TYPE, 0);

		gpu.textures.unbind(0);

		gpu.programs.pop();

		gpu.buffers.pop(Gpu::BufferTarget::ELEMENT_ARRAY);
		gpu.buffers.pop(Gpu::BufferTarget::ARRAY);
	}

	void render_axes(const mat4& world_matrix, const mat4& view_projection_matrix);

	template<typename T>
	void render_aabb(const mat4& world_matrix, const mat4& view_projection_matrix, const details::AABB3<T>& aabb, const vec4& color)
	{
		typedef VertexBuffer<basic_gpu_program::VertexType> VertexBufferType;
		typedef IndexBuffer<u8> IndexBufferType;
		typedef VertexBufferType::VertexType VertexType;

		static boost::weak_ptr<VertexBufferType> vertex_buffer;
		static boost::weak_ptr<IndexBufferType> index_buffer;

		if (vertex_buffer.expired())
		{
			vertex_buffer = gpu_buffers.make<VertexBufferType>();
			auto vertices = {
				VertexType(vec3(0, 0, 0), vec4(1)),
				VertexType(vec3(1, 0, 0), vec4(1)),
				VertexType(vec3(1, 1, 0), vec4(1)),
				VertexType(vec3(0, 1, 0), vec4(1)),
				VertexType(vec3(0, 0, 1), vec4(1)),
				VertexType(vec3(1, 0, 1), vec4(1)),
				VertexType(vec3(1, 1, 1), vec4(1)),
				VertexType(vec3(0, 1, 1), vec4(1))
			};
			vertex_buffer.lock()->data(vertices, Gpu::BufferUsage::DYNAMIC_DRAW);
		}

		if (index_buffer.expired())
		{
			index_buffer = gpu_buffers.make<IndexBufferType>();
			index_buffer.lock()->data({ 0, 1, 1, 2, 2, 3, 3, 0, 0, 4, 1, 5, 2, 6, 3, 7, 4, 5, 5, 6, 6, 7, 7, 4 }, Gpu::BufferUsage::STATIC_DRAW);
		}

		gpu.buffers.push(Gpu::BufferTarget::ARRAY, vertex_buffer.lock());
		gpu.buffers.push(Gpu::BufferTarget::ELEMENT_ARRAY, index_buffer.lock());

		const auto gpu_program = gpu_programs.get<basic_gpu_program>();

		gpu.programs.push(gpu_program);

		gpu.set_uniform("world_matrix", world_matrix * glm::translate(aabb.min) * glm::scale(aabb.size()));
		gpu.set_uniform("view_projection_matrix", view_projection_matrix);
		gpu.set_uniform("color", color);

		// TODO: have this as some sort of constant somewhere!
		auto texture = resources.get<Texture>("white.png");
		gpu.textures.bind(0, texture);

		gpu.draw_elements(Gpu::PrimitiveType::LINES, 24, IndexBufferType::DATA_TYPE, 0);

		gpu.textures.unbind(0);

		gpu.programs.pop();

		gpu.buffers.pop(Gpu::BufferTarget::ELEMENT_ARRAY);
		gpu.buffers.pop(Gpu::BufferTarget::ARRAY);
	}

	template<typename T>
	void render_sphere(const mat4& world_matrix, const mat4& view_projection_matrix, const details::Sphere<T>& sphere, const vec4& color)
	{
		typedef VertexBuffer<basic_gpu_program::VertexType> VertexBufferType;
		typedef IndexBuffer<u8> IndexBufferType;

		static boost::weak_ptr<VertexBufferType> vertex_buffer;
		static boost::weak_ptr<IndexBufferType> index_buffer;

		const size_t SPHERE_SIDES = 32;

		if (vertex_buffer.expired())
		{
			vertex_buffer = gpu_buffers.make<VertexBufferType>();

			std::vector<basic_gpu_program::VertexType> vertices;
			vertices.reserve(SPHERE_SIDES);

			//TODO: this is lame, write this better
			for (auto i = 0; i < SPHERE_SIDES; ++i)
			{
				auto sigma = static_cast<f32>(i) / SPHERE_SIDES * glm::pi<f32>() * 2;

				vertices.emplace_back(vec3(glm::sin(sigma), glm::cos(sigma), 0), vec4(0, 0, 1, 1));
			}

			for (auto i = 0; i < SPHERE_SIDES; ++i)
			{
				auto sigma = static_cast<f32>(i) / SPHERE_SIDES * glm::pi<f32>() * 2;

				vertices.emplace_back(vec3(0, glm::sin(sigma), glm::cos(sigma)), vec4(1, 0, 0, 1));
			}

			for (auto i = 0; i < SPHERE_SIDES; ++i)
			{
				auto sigma = static_cast<f32>(i) / SPHERE_SIDES * glm::pi<f32>() * 2;

				vertices.emplace_back(vec3(glm::cos(sigma), 0, glm::sin(sigma)), vec4(0, 1, 0, 1));
			}

			vertex_buffer.lock()->data(vertices, Gpu::BufferUsage::DYNAMIC_DRAW);
		}

		if (index_buffer.expired())
		{
			index_buffer = gpu_buffers.make<IndexBufferType>();

			std::vector<u8> indices;
			indices.reserve(SPHERE_SIDES * 3);

			for (auto i = 0; i < SPHERE_SIDES * 3; ++i)
			{
				indices.push_back(i);
			}

			index_buffer.lock()->data(indices, Gpu::BufferUsage::STATIC_DRAW);
		}

		gpu.buffers.push(Gpu::BufferTarget::ARRAY, vertex_buffer.lock());
		gpu.buffers.push(Gpu::BufferTarget::ELEMENT_ARRAY, index_buffer.lock());

		const auto gpu_program = gpu_programs.get<basic_gpu_program>();

		gpu.programs.push(gpu_program);

		gpu.set_uniform("world_matrix", world_matrix * glm::translate(sphere.origin) * glm::scale(vec3(sphere.radius)));
		gpu.set_uniform("view_projection_matrix", view_projection_matrix);
		gpu.set_uniform("color", color);

		// TODO: have this as some sort of constant somewhere!
		auto texture = resources.get<Texture>("white.png");
		gpu.textures.bind(0, texture);

		gpu.draw_elements(Gpu::PrimitiveType::LINE_LOOP, SPHERE_SIDES, IndexBufferType::DATA_TYPE, SPHERE_SIDES * 0);
		gpu.draw_elements(Gpu::PrimitiveType::LINE_LOOP, SPHERE_SIDES, IndexBufferType::DATA_TYPE, SPHERE_SIDES * 1);
		gpu.draw_elements(Gpu::PrimitiveType::LINE_LOOP, SPHERE_SIDES, IndexBufferType::DATA_TYPE, SPHERE_SIDES * 2);

		gpu.textures.unbind(0);

		gpu.programs.pop();

		gpu.buffers.pop(Gpu::BufferTarget::ELEMENT_ARRAY);
		gpu.buffers.pop(Gpu::BufferTarget::ARRAY);
	}
}