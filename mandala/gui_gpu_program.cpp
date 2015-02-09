//mandala
#include "gui_gpu_program.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
	std::string gui_gpu_program_t::vertex_shader_source = R"(
#version 150

precision lowp float;

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;

in vec3 location;
in vec2 texcoord;

out vec2 out_texcoord;

void main() 
{
	out_texcoord = texcoord;
	
	gl_Position = (view_projection_matrix) * (world_matrix * vec4(location, 1));
}
)";

	std::string gui_gpu_program_t::fragment_shader_source = R"(
#version 150

uniform sampler2D diffuse_texture;
uniform vec4 color;

in vec2 out_texcoord;

out vec4 fragment;

void main()
{
	fragment = texture2D(diffuse_texture, out_texcoord) * color;
}
)";

	gui_gpu_program_t::gui_gpu_program_t() :
		gpu_program_t(vertex_shader_source, fragment_shader_source)
	{
        location_location = gpu.get_attribute_location(get_id(), "location");
		texcoord_location = gpu.get_attribute_location(get_id(), "texcoord");

		world_matrix_location = gpu.get_uniform_location(get_id(), "world_matrix");
		view_projection_matrix_location = gpu.get_uniform_location(get_id(), "view_projection_matrix");
		diffuse_texture_index_location = gpu.get_uniform_location(get_id(), "diffuse_texture");
		color_location = gpu.get_uniform_location(get_id(), "color");
	}

	void gui_gpu_program_t::on_bind()
	{
		static const auto location_offset = reinterpret_cast<void*>(offsetof(vertex_type, location));
		static const auto texcoord_offset = reinterpret_cast<void*>(offsetof(vertex_type, texcoord));

		gpu.enable_vertex_attribute_array(location_location);
		gpu.enable_vertex_attribute_array(texcoord_location);

		gpu.set_vertex_attrib_pointer(location_location, 2, gpu_data_type_e::float_, false, sizeof(vertex_type), location_offset);
		gpu.set_vertex_attrib_pointer(texcoord_location, 2, gpu_data_type_e::float_, false, sizeof(vertex_type), texcoord_offset);

		//uniforms
	}

	void gui_gpu_program_t::on_unbind()
	{
		gpu.disable_vertex_attribute_array(location_location);
		gpu.disable_vertex_attribute_array(texcoord_location);
	}

	void gui_gpu_program_t::world_matrix(const mat4_t& world_matrix) const
	{
		gpu.set_uniform(world_matrix_location, world_matrix, false);
	}

	void gui_gpu_program_t::view_projection_matrix(const mat4_t& view_projection_matrix) const
	{
		gpu.set_uniform(view_projection_matrix_location, view_projection_matrix, false);
	}

	void gui_gpu_program_t::diffuse_texture_index(uint32_t diffuse_texture_index) const
	{
		gpu.set_uniform(diffuse_texture_index_location, diffuse_texture_index);
	}

	void gui_gpu_program_t::color(const vec4_t& color) const
	{
		gpu.set_uniform(color_location, color);
	}
}
