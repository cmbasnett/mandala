#include "psk.hpp"
#include "io.hpp"
#include "model.hpp"
#include "gpu.hpp"
#include "camera_params.hpp"
#include "gpu_program_mgr.hpp"
#include "gpu_buffer_mgr.hpp"
#include "resource_manager.hpp"
#include "texture.hpp"
#include "opengl.hpp"

#include <iostream>
#include <fstream>

namespace naga
{
	PSK::PSK(const std::string& path)
	{
		auto istream = std::ifstream(path, std::ios::binary);

		while (true)
		{
			ChunkHeader chunk_header;
			read(istream, chunk_header);

			if (istream.eof())
			{
				break;
			}

			if (strcmp(chunk_header.chunk_id, "ACTRHEAD") == 0)
			{
				continue;
			}
			else if (strcmp(chunk_header.chunk_id, "PNTS0000") == 0)
			{
				read(istream, points, chunk_header.data_count);
			}
			else if (strcmp(chunk_header.chunk_id, "VTXW0000") == 0)
			{
				if (chunk_header.data_count <= 65536)
				{
					std::vector<PaddedVertex> padded_vertices;
					read(istream, padded_vertices, chunk_header.data_count);
					vertices.reserve(chunk_header.data_count);

					for (auto& padded_vertex : padded_vertices)
					{
						Vertex vertex;
						vertex.point_index = padded_vertex.point_index;
						vertex.material_index = padded_vertex.material_index;
						vertex.u = padded_vertex.u;
						vertex.v = padded_vertex.v;
						vertices.push_back(vertex);
					}
				}
				else
				{
					read(istream, vertices, chunk_header.data_count);
				}
			}
			else if (strcmp(chunk_header.chunk_id, "FACE0000") == 0)
			{
				read(istream, faces, chunk_header.data_count);
			}
			else if (strcmp(chunk_header.chunk_id, "MATT0000") == 0)
			{
				read(istream, materials, chunk_header.data_count);
			}
			else if (strcmp(chunk_header.chunk_id, "REFSKELT") == 0)
			{
				read(istream, bones, chunk_header.data_count);
			}
			else if (strcmp(chunk_header.chunk_id, "RAWWEIGHTS") == 0)
			{
				read(istream, weights, chunk_header.data_count);
			}
			else if (strcmp(chunk_header.chunk_id, "EXTRAUV0") == 0)
			{
				// TODO: more extra-UVs
			}
			else
			{
				throw std::exception("Invalid chunk header!");
			}
		}

		// vertex buffer
		std::vector<vertex_type> vertices;
		vertices.reserve(this->vertices.size());

		for (size_t i = 0; i < this->vertices.size(); ++i)
		{
			const auto& v = this->vertices[i];
			vertex_type vertex;
			vertex.location = points[v.point_index].swizzle(glm::comp::X, glm::comp::Z, glm::comp::Y);
			vertex.color = vec4(f32(i) / this->vertices.size());
			vertices.push_back(vertex);
		}

		vertex_buffer = gpu_buffers.make<VertexBufferType>().lock();
		vertex_buffer->data(vertices, Gpu::BufferUsage::STATIC_DRAW);

		// index buffer
		std::vector<index_type> indices;

		for (auto& face : faces)
		{
			indices.push_back(face.vertex_indices[0]);
			indices.push_back(face.vertex_indices[1]);
			indices.push_back(face.vertex_indices[2]);
		}

		index_count = indices.size();

		index_buffer = gpu_buffers.make<IndexBufferType>().lock();
		index_buffer->data(indices, Gpu::BufferUsage::STATIC_DRAW);
	}

	void PSK::render(CameraParameters& camera_parameters)
	{
		gpu.buffers.push(Gpu::BufferTarget::ARRAY, vertex_buffer);
		gpu.buffers.push(Gpu::BufferTarget::ELEMENT_ARRAY, index_buffer);

		const auto gpu_program = gpu_programs.get<basic_gpu_program>();

		gpu.programs.push(gpu_program);

		gpu.set_uniform("world_matrix", mat4());
		gpu.set_uniform("view_projection_matrix", camera_parameters.projection_matrix * camera_parameters.view_matrix);
		//gpu.set_uniform("diffuse_texture", 0);
		gpu.set_uniform("color", vec4(1.0f));


		const auto texture = resources.get<Texture>("white.png");
		gpu.textures.bind(0, texture);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		gpu.draw_elements(Gpu::PrimitiveType::TRIANGLES, index_count, IndexBufferType::DATA_TYPE, 0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		gpu.textures.unbind(0);

		gpu.programs.pop();

		gpu.buffers.pop(Gpu::BufferTarget::ELEMENT_ARRAY);
		gpu.buffers.pop(Gpu::BufferTarget::ARRAY);
	}
}