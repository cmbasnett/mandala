//mandala
#include "bitmap_font_gpu_program.hpp"
#include "opengl.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
	std::string bitmap_font_gpu_program_t::vertex_shader_source = R"(
#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

precision lowp float;

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;
uniform float line_height;
uniform float base;
uniform vec4 color_top;
uniform vec4 color_bottom;

in vec2 position;
in vec2 texcoord;

out vec2 out_texcoord;
out vec4 out_color;

void main() 
{
	gl_Position = (view_projection_matrix) * (world_matrix * vec4(position, 0, 1));
	
	out_texcoord = texcoord;
	
	float t = (position.y + (line_height - base)) / line_height;
	out_color = mix(color_bottom, color_top, t);
}
)";

	std::string bitmap_font_gpu_program_t::fragment_shader_source = R"(
#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

uniform sampler2D diffuse_texture;

in vec2 out_texcoord;
in vec4 out_color;

void main() 
{
	gl_FragColor = texture2D(diffuse_texture, out_texcoord) * out_color;
}
)";

	bitmap_font_gpu_program_t::bitmap_font_gpu_program_t() :
		gpu_program_t(vertex_shader_source, fragment_shader_source)
	{
		position_location = glGetAttribLocation(id, "position"); glCheckError();
		texcoord_location = glGetAttribLocation(id, "texcoord"); glCheckError();

		world_matrix_location = glGetUniformLocation(id, "world_matrix"); glCheckError();
		view_projection_matrix_location = glGetUniformLocation(id, "view_projection_matrix"); glCheckError();
		font_line_height_location = glGetUniformLocation(id, "line_height"); glCheckError();
		font_base_location = glGetUniformLocation(id, "base"); glCheckError();
		font_color_top_location = glGetUniformLocation(id, "color_top"); glCheckError();
		font_color_bottom_location = glGetUniformLocation(id, "color_bottom"); glCheckError();
		diffuse_texture_index_location = glGetUniformLocation(id, "diffuse_texture"); glCheckError();
	}

	void bitmap_font_gpu_program_t::on_bind()
	{
		glEnableVertexAttribArray(position_location); glCheckError();
		glEnableVertexAttribArray(texcoord_location); glCheckError();

		glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, position))); glCheckError();
		glVertexAttribPointer(texcoord_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, texcoord))); glCheckError();
	}

	void bitmap_font_gpu_program_t::on_unbind()
	{
		glDisableVertexAttribArray(position_location); glCheckError();
		glDisableVertexAttribArray(texcoord_location); glCheckError();
	}

	void bitmap_font_gpu_program_t::world_matrix(const mat4_t& world_matrix) const
	{
		glUniformMatrix4fv(world_matrix_location, 1, GL_FALSE, glm::value_ptr(world_matrix)); glCheckError();
	}

	void bitmap_font_gpu_program_t::view_projection_matrix(const mat4_t& view_projection_matrix) const
	{
		glUniformMatrix4fv(view_projection_matrix_location, 1, GL_FALSE, glm::value_ptr(view_projection_matrix)); glCheckError();
	}

	void bitmap_font_gpu_program_t::diffuse_texture_index(uint32_t diffuse_texture_index) const
	{
		glUniform1i(diffuse_texture_index_location, diffuse_texture_index);
	}

	void bitmap_font_gpu_program_t::font_line_height(float32_t font_line_height) const
	{
		glUniform1f(font_line_height_location, font_line_height); glCheckError();
	}

	void bitmap_font_gpu_program_t::font_base(float32_t font_base) const
	{
		glUniform1f(font_base_location, font_base); glCheckError();
	}

	void bitmap_font_gpu_program_t::font_diffuse_texture_index(uint32_t font_diffuse_texture_index) const
	{
		glUniform1i(font_diffuse_texture_index_location, font_diffuse_texture_index); glCheckError();
	}

	void bitmap_font_gpu_program_t::font_color_top(const vec4_t& font_color_top) const
	{
		glUniform4fv(font_color_top_location, 1, glm::value_ptr(font_color_top)); glCheckError();
	}

	void bitmap_font_gpu_program_t::font_color_bottom(const vec4_t& font_color_bottom) const
	{
		glUniform4fv(font_color_bottom_location, 1, glm::value_ptr(font_color_bottom)); glCheckError();
	}
}
