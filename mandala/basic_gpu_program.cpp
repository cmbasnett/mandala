//mandala
#include "basic_gpu_program.hpp"
#include "opengl.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
	std::shared_ptr<basic_gpu_program_t> basic_gpu_program;

	std::string basic_gpu_program_t::vertex_shader_source = R"(
#version 150

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;

in vec3 position;
in vec4 color;

out vec4 out_color;

void main() 
{
	gl_Position = view_projection_matrix * (world_matrix * vec4(position, 1));

	out_color = color;
})";

	std::string basic_gpu_program_t::fragment_shader_source = R"(
#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

in vec4 out_color;

out vec4 fragment;

void main() 
{
	fragment = out_color;
})";

	basic_gpu_program_t::basic_gpu_program_t() :
		gpu_program_t(vertex_shader_source, fragment_shader_source)
	{
		position_location = glGetAttribLocation(id, "position"); glCheckError();
		color_location = glGetAttribLocation(id, "color"); glCheckError();

		world_matrix_location = glGetUniformLocation(id, "world_matrix"); glCheckError();
		view_projection_matrix_location = glGetUniformLocation(id, "view_projection_matrix"); glCheckError();
	}

	void basic_gpu_program_t::on_bind()
	{
		static const auto position_offset = reinterpret_cast<void*>(offsetof(vertex_type, position));
		static const auto color_offset = reinterpret_cast<void*>(offsetof(vertex_type, color));

		glEnableVertexAttribArray(position_location); glCheckError();
		glEnableVertexAttribArray(color_location); glCheckError();

		glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_type), position_offset); glCheckError();
		glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_type), color_offset); glCheckError();
	}

	void basic_gpu_program_t::on_unbind()
	{
		glDisableVertexAttribArray(position_location); glCheckError();
		glDisableVertexAttribArray(color_location); glCheckError();
	}

	void basic_gpu_program_t::world_matrix(const mat4_t& world_matrix) const
	{
		glUniformMatrix4fv(world_matrix_location, 1, GL_FALSE, glm::value_ptr(world_matrix)); glCheckError();
	}

	void basic_gpu_program_t::view_projection_matrix(const mat4_t& view_projection_matrix) const
	{
		glUniformMatrix4fv(view_projection_matrix_location, 1, GL_FALSE, glm::value_ptr(view_projection_matrix)); glCheckError();
	}
}
