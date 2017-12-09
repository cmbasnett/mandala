//naga
#include "debug_renderer.hpp"

namespace naga
{
	void render_axes(const mat4& world_matrix, const mat4& view_projection_matrix)
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
				VertexType(vec3(0, 0, 0), vec4(1, 0, 0, 1)),
				VertexType(vec3(1, 0, 0), vec4(1, 0, 0, 1)),
				VertexType(vec3(0, 0, 0), vec4(0, 1, 0, 1)),
				VertexType(vec3(0, 1, 0), vec4(0, 1, 0, 1)),
				VertexType(vec3(0, 0, 0), vec4(0, 0, 1, 1)),
				VertexType(vec3(0, 0, 1), vec4(0, 0, 1, 1))
			};
			vertex_buffer.lock()->data(vertices, Gpu::BufferUsage::STATIC_DRAW);
		}

		if (index_buffer.expired())
		{
			index_buffer = gpu_buffers.make<IndexBufferType>();
			index_buffer.lock()->data({ 0, 1, 2, 3, 4, 5 }, Gpu::BufferUsage::STATIC_DRAW);
		}

		gpu.buffers.push(Gpu::BufferTarget::ARRAY, vertex_buffer.lock());
		gpu.buffers.push(Gpu::BufferTarget::ELEMENT_ARRAY, index_buffer.lock());

		const auto gpu_program = gpu_programs.get<basic_gpu_program>();

		gpu.programs.push(gpu_program);

		gpu.set_uniform("world_matrix", world_matrix);
		gpu.set_uniform("view_projection_matrix", view_projection_matrix);
		gpu.set_uniform("color", vec4(1));

		// TODO: have this as some sort of constant somewhere!
		auto texture = resources.get<Texture>("white.png");
		gpu.textures.bind(0, texture);

		gpu.draw_elements(Gpu::PrimitiveType::LINES, 6, IndexBufferType::DATA_TYPE, 0);

		gpu.textures.unbind(0);

		gpu.programs.pop();

		gpu.buffers.pop(Gpu::BufferTarget::ELEMENT_ARRAY);
		gpu.buffers.pop(Gpu::BufferTarget::ARRAY);
	}
}