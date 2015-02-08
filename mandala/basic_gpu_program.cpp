//mandala
#include "basic_gpu_program.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
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
		location_location = gpu.get_attribute_location(id(), "location");
		color_location = gpu.get_attribute_location(id(), "color");

		world_matrix_location = gpu.get_uniform_location(id(), "world_matrix");
		view_projection_matrix_location = gpu.get_uniform_location(id(), "view_projection_matrix");
	}

	void basic_gpu_program_t::on_bind()
	{
		static const auto location_offset = reinterpret_cast<void*>(offsetof(vertex_type, location));
		static const auto color_offset = reinterpret_cast<void*>(offsetof(vertex_type, color));

		gpu.enable_vertex_attribute_array(location_location);
		gpu.enable_vertex_attribute_array(color_location);

		gpu.set_vertex_attrib_pointer(location_location, 3, gpu_data_type_e::float_, false, sizeof(vertex_type), location_offset);
		gpu.set_vertex_attrib_pointer(color_location, 4, gpu_data_type_e::float_, false, sizeof(vertex_type), color_offset);
	}

	void basic_gpu_program_t::on_unbind()
	{
		gpu.disable_vertex_attribute_array(location_location);
		gpu.disable_vertex_attribute_array(color_location);
	}

	void basic_gpu_program_t::world_matrix(const mat4_t& world_matrix) const
	{
		gpu.set_uniform(world_matrix_location, world_matrix, false);
	}

	void basic_gpu_program_t::view_projection_matrix(const mat4_t& view_projection_matrix) const
	{
		gpu.set_uniform(view_projection_matrix_location, view_projection_matrix, false);
	}
}
