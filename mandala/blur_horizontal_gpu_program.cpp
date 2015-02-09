//mandala
#include "blur_horizontal_gpu_program.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
    std::string blur_horizontal_gpu_program_t::vertex_shader_source = R"(
#version 150

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;

in vec3 location;
in vec2 texcoord;

out vec2 out_texcoord;

void main(void)
{
    out_texcoord = texcoord;

	gl_Position = (view_projection_matrix) * (world_matrix * vec4(location, 1));
}
)";

    std::string blur_horizontal_gpu_program_t::fragment_shader_source = R"(
#version 150

uniform sampler2D diffuse_texture;
uniform vec2 texture_size;
uniform float blur_size = 1.0 / 512.0;

in vec2 out_texcoord;

out vec4 fragment;

void main()
{
    fragment += texture2D(diffuse_texture, vec2(out_texcoord.x - 4.0 * blur_size, out_texcoord.y)) * 0.05;
    fragment += texture2D(diffuse_texture, vec2(out_texcoord.x - 3.0 * blur_size, out_texcoord.y)) * 0.09;
    fragment += texture2D(diffuse_texture, vec2(out_texcoord.x - 2.0 * blur_size, out_texcoord.y)) * 0.12;
    fragment += texture2D(diffuse_texture, vec2(out_texcoord.x - blur_size,       out_texcoord.y)) * 0.15;
    fragment += texture2D(diffuse_texture, vec2(out_texcoord.x,                   out_texcoord.y)) * 0.16;
    fragment += texture2D(diffuse_texture, vec2(out_texcoord.x + blur_size,       out_texcoord.y)) * 0.15;
    fragment += texture2D(diffuse_texture, vec2(out_texcoord.x + 2.0 * blur_size, out_texcoord.y)) * 0.12;
    fragment += texture2D(diffuse_texture, vec2(out_texcoord.x + 3.0 * blur_size, out_texcoord.y)) * 0.09;
    fragment += texture2D(diffuse_texture, vec2(out_texcoord.x + 4.0 * blur_size, out_texcoord.y)) * 0.05;
    fragment += noise4(out_texcoord.x + out_texcoord.y);
}
)";

	blur_horizontal_gpu_program_t::blur_horizontal_gpu_program_t() :
		gpu_program_t(vertex_shader_source, fragment_shader_source)
	{
		location_location = gpu.get_attribute_location(get_id(), "location");
		texcoord_location = gpu.get_attribute_location(get_id(), "texcoord");

		world_matrix_location = gpu.get_uniform_location(get_id(), "world_matrix");
		view_projection_matrix_location = gpu.get_uniform_location(get_id(), "view_projection_matrix");
		blur_size_location = gpu.get_uniform_location(get_id(), "blur_size");
		diffuse_texture_index_location = gpu.get_uniform_location(get_id(), "diffuse_texture");
	}

	void blur_horizontal_gpu_program_t::on_bind()
	{
		gpu.enable_vertex_attribute_array(location_location);
		gpu.enable_vertex_attribute_array(texcoord_location);

		gpu.set_vertex_attrib_pointer(location_location, 3, gpu_data_type_e::float_, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, location)));
		gpu.set_vertex_attrib_pointer(texcoord_location, 2, gpu_data_type_e::float_, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, texcoord)));
	}

	void blur_horizontal_gpu_program_t::on_unbind()
	{
		gpu.disable_vertex_attribute_array(location_location);
		gpu.disable_vertex_attribute_array(texcoord_location);
	}

	void blur_horizontal_gpu_program_t::world_matrix(const mat4_t& world_matrix) const
	{
		gpu.set_uniform(world_matrix_location, world_matrix, false);
	}

	void blur_horizontal_gpu_program_t::view_projection_matrix(const mat4_t& view_projection_matrix) const
	{
		gpu.set_uniform(view_projection_matrix_location, view_projection_matrix, false);
	}

	void blur_horizontal_gpu_program_t::diffuse_texture_index(uint32_t diffuse_texture_index) const
	{
		gpu.set_uniform(diffuse_texture_index_location, diffuse_texture_index);
	}

	void blur_horizontal_gpu_program_t::blur_size(float32_t blur_size) const
	{
		gpu.set_uniform(blur_size_location, blur_size);
	}
}
