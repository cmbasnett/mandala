//mandala
#include "bitmap_font_gpu_program.hpp"

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
uniform int should_invert_rgb;

in vec2 location;
in vec2 texcoord;

out vec2 out_texcoord;
out vec4 out_color;

void main() 
{
	gl_Position = (view_projection_matrix) * (world_matrix * vec4(location, 0, 1));
	
	out_texcoord = texcoord;
	
	float t = (location.y + (line_height - base)) / line_height;
	out_color = mix(color_bottom, color_top, t);

    //TODO: convert to color modifier subroutine
    if (should_invert_rgb == 1)
    {
        out_color.rgb = vec3(1) - out_color.rgb;
    }
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
		location_location = gpu.get_attribute_location(id(), "location");
		texcoord_location = gpu.get_attribute_location(id(), "texcoord");

		world_matrix_location = gpu.get_uniform_location(id(), "world_matrix");
		view_projection_matrix_location = gpu.get_uniform_location(id(), "view_projection_matrix");
		font_line_height_location = gpu.get_uniform_location(id(), "line_height");
		font_base_location = gpu.get_uniform_location(id(), "base");
		font_color_top_location = gpu.get_uniform_location(id(), "color_top");
		font_color_bottom_location = gpu.get_uniform_location(id(), "color_bottom");
		diffuse_texture_index_location = gpu.get_uniform_location(id(), "diffuse_texture");
        should_invert_rgb_location = gpu.get_uniform_location(id(), "should_invert_rgb");
	}

	void bitmap_font_gpu_program_t::on_bind()
	{
		static const auto location_offset = reinterpret_cast<void*>(offsetof(vertex_type, location));
		static const auto texcoord_offset = reinterpret_cast<void*>(offsetof(vertex_type, texcoord));

		gpu.enable_vertex_attribute_array(location_location);
		gpu.enable_vertex_attribute_array(texcoord_location);

		gpu.set_vertex_attrib_pointer(location_location, 2, gpu_data_type_e::float_, false, sizeof(vertex_type), location_offset);
		gpu.set_vertex_attrib_pointer(texcoord_location, 2, gpu_data_type_e::float_, false, sizeof(vertex_type), texcoord_offset);
	}

	void bitmap_font_gpu_program_t::on_unbind()
	{
		gpu.disable_vertex_attribute_array(location_location);
		gpu.disable_vertex_attribute_array(texcoord_location);
	}

	void bitmap_font_gpu_program_t::world_matrix(const mat4_t& world_matrix) const
	{
		gpu.set_uniform(world_matrix_location, world_matrix, false);
	}

	void bitmap_font_gpu_program_t::view_projection_matrix(const mat4_t& view_projection_matrix) const
	{
		gpu.set_uniform(view_projection_matrix_location, view_projection_matrix, false);
	}

	void bitmap_font_gpu_program_t::diffuse_texture_index(uint32_t diffuse_texture_index) const
	{
		gpu.set_uniform(diffuse_texture_index_location, diffuse_texture_index);
	}

	void bitmap_font_gpu_program_t::font_line_height(float32_t font_line_height) const
	{
		gpu.set_uniform(font_line_height_location, font_line_height);
	}

	void bitmap_font_gpu_program_t::font_base(float32_t font_base) const
	{
		gpu.set_uniform(font_base_location, font_base);
	}

	void bitmap_font_gpu_program_t::font_diffuse_texture_index(uint32_t font_diffuse_texture_index) const
	{
		gpu.set_uniform(font_diffuse_texture_index_location, font_diffuse_texture_index);
	}

	void bitmap_font_gpu_program_t::font_color_top(const vec4_t& font_color_top) const
	{
		gpu.set_uniform(font_color_top_location, font_color_top);
	}

	void bitmap_font_gpu_program_t::font_color_bottom(const vec4_t& font_color_bottom) const
	{
		gpu.set_uniform(font_color_bottom_location, font_color_bottom);
	}

    void bitmap_font_gpu_program_t::should_invert_rgb(bool should_invert_rgb) const
    {
		gpu.set_uniform(should_invert_rgb_location, should_invert_rgb ? 1 : 0);
    }
}
