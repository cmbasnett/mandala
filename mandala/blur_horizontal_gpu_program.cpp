//mandala
#include "blur_horizontal_gpu_program.hpp"
#include "opengl.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
	std::string blur_horizontal_gpu_program_t::vertex_shader_source = R"(
#version 150


uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;

attribute vec2 position;
attribute vec2 texcoord;

out vec2 out_texcoords[5];

void main(void)
{
	vec2 singleStepOffset = vec2(0.005, 0.005);
	out_texcoords[0] = texcoord.xy;
	out_texcoords[1] = texcoord.xy + singleStepOffset * 1.407333;
	out_texcoords[2] = texcoord.xy - singleStepOffset * 1.407333;
	out_texcoords[3] = texcoord.xy + singleStepOffset * 3.294215;
	out_texcoords[4] = texcoord.xy - singleStepOffset * 3.294215;

	gl_Position = (view_projection_matrix) * (world_matrix * vec4(position, 0, 1));
}
)";

	std::string blur_horizontal_gpu_program_t::fragment_shader_source = R"(
#version 150

uniform sampler2D diffuse_texture;
uniform float blur_size;

varying highp vec2 out_texcoords[5];

out vec4 fragment;

void main() 
{
	fragment += texture2D(diffuse_texture, out_texcoords[0]) * 0.204164;
	fragment += texture2D(diffuse_texture, out_texcoords[1]) * 0.304005;
	fragment += texture2D(diffuse_texture, out_texcoords[2]) * 0.304005;
	fragment += texture2D(diffuse_texture, out_texcoords[3]) * 0.093913;
	fragment += texture2D(diffuse_texture, out_texcoords[4]) * 0.093913;
}
)";

	blur_horizontal_gpu_program_t::blur_horizontal_gpu_program_t() :
		gpu_program_t(vertex_shader_source, fragment_shader_source)
	{
		position_location = glGetAttribLocation(id, "position"); glCheckError();
		texcoord_location = glGetAttribLocation(id, "texcoord"); glCheckError();

		world_matrix_location = glGetUniformLocation(id, "world_matrix"); glCheckError();
		view_projection_matrix_location = glGetUniformLocation(id, "view_projection_matrix"); glCheckError();
		blur_size_location = glGetUniformLocation(id, "blur_size"); glCheckError();
		diffuse_texture_index_location = glGetUniformLocation(id, "diffuse_texture");
	}

	void blur_horizontal_gpu_program_t::on_bind()
	{
		glEnableVertexAttribArray(position_location); glCheckError();
		glEnableVertexAttribArray(texcoord_location); glCheckError();

		glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, position))); glCheckError();
		glVertexAttribPointer(texcoord_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, texcoord))); glCheckError();
	}

	void blur_horizontal_gpu_program_t::on_unbind()
	{
		glDisableVertexAttribArray(position_location); glCheckError();
		glDisableVertexAttribArray(texcoord_location); glCheckError();
	}

	void blur_horizontal_gpu_program_t::world_matrix(const mat4_t& world_matrix) const
	{
		glUniformMatrix4fv(world_matrix_location, 1, GL_FALSE, glm::value_ptr(world_matrix)); glCheckError();
	}

	void blur_horizontal_gpu_program_t::view_projection_matrix(const mat4_t& view_projection_matrix) const
	{
		glUniformMatrix4fv(view_projection_matrix_location, 1, GL_FALSE, glm::value_ptr(view_projection_matrix)); glCheckError();
	}

	void blur_horizontal_gpu_program_t::diffuse_texture_index(uint32_t diffuse_texture_index) const
	{
		glUniform1i(diffuse_texture_index_location, diffuse_texture_index);
	}

	void blur_horizontal_gpu_program_t::blur_size(float32_t blur_size) const
	{
		glUniform1f(blur_size_location, blur_size);
	}
}
