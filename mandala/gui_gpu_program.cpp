//mandala
#include "gui_gpu_program.hpp"
#include "opengl.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
	std::shared_ptr<gui_gpu_program_t> gui_gpu_program;

	std::string gui_gpu_program_t::vertex_shader_source = R"(
#version 150

precision lowp float;

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;

in vec3 position;
in vec2 texcoord;

out vec2 out_texcoord;

void main() 
{
	out_texcoord = texcoord;
	
	gl_Position = (view_projection_matrix) * (world_matrix * vec4(position, 1));
}
)";

	std::string gui_gpu_program_t::fragment_shader_source = R"(
#version 150

uniform sampler2D diffuse_texture;

in vec2 out_texcoord;

out vec4 fragment;

void main() 
{
	vec4 co = texture2D(diffuse_texture, out_texcoord);

	fragment = co + fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453) * 0.1;
}
)";

	gui_gpu_program_t::gui_gpu_program_t() :
		gpu_program_t(vertex_shader_source, fragment_shader_source)
	{
		position_location = glGetAttribLocation(id, "position"); glCheckError();
		texcoord_location = glGetAttribLocation(id, "texcoord"); glCheckError();

		world_matrix_location = glGetUniformLocation(id, "world_matrix"); glCheckError();
		view_projection_matrix_location = glGetUniformLocation(id, "view_projection_matrix"); glCheckError();
		diffuse_texture_index_location = glGetUniformLocation(id, "diffuse_texture"); glCheckError();
	}

	void gui_gpu_program_t::on_bind()
	{
		static const auto position_offset = reinterpret_cast<void*>(offsetof(vertex_type, position));
		static const auto texcoord_offset = reinterpret_cast<void*>(offsetof(vertex_type, texcoord));

		glEnableVertexAttribArray(position_location); glCheckError();
		glEnableVertexAttribArray(texcoord_location); glCheckError();

		glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_type), position_offset); glCheckError();
		glVertexAttribPointer(texcoord_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_type), texcoord_offset); glCheckError();

		//uniforms
	}

	void gui_gpu_program_t::on_unbind()
	{
		glDisableVertexAttribArray(position_location); glCheckError();
		glDisableVertexAttribArray(texcoord_location); glCheckError();
	}

	void gui_gpu_program_t::world_matrix(const mat4_t& world_matrix) const
	{
		glUniformMatrix4fv(world_matrix_location, 1, GL_FALSE, glm::value_ptr(world_matrix)); glCheckError();
	}

	void gui_gpu_program_t::view_projection_matrix(const mat4_t& view_projection_matrix) const
	{
		glUniformMatrix4fv(view_projection_matrix_location, 1, GL_FALSE, glm::value_ptr(view_projection_matrix)); glCheckError();
	}

	void gui_gpu_program_t::diffuse_texture_index(uint32_t diffuse_texture_index) const
	{
		glUniform1i(diffuse_texture_index_location, diffuse_texture_index); glCheckError();
	}
}
