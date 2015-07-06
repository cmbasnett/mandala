//mandala
#include "bsp_gpu_program.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
	std::string bsp_gpu_program_t::vertex_shader_source = R"(
#version 400

precision lowp float;

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;
uniform sampler2D diffuse_texture;
uniform sampler2D lightmap_texture;

in vec3 location;
in vec2 diffuse_texcoord;
in vec2 lightmap_texcoord;

out vec2 out_diffuse_texcoord;
out vec2 out_lightmap_texcoord;

void main() 
{
	gl_Position = view_projection_matrix * (world_matrix * vec4(location, 1));
	
	//out_normal = normal;
	out_diffuse_texcoord = diffuse_texcoord;
	out_lightmap_texcoord = lightmap_texcoord;
})";

	std::string bsp_gpu_program_t::fragment_shader_source = R"(
#version 400

uniform sampler2D diffuse_texture;
uniform sampler2D lightmap_texture;
uniform float lightmap_gamma;
uniform float alpha;
uniform bool should_test_alpha;

in vec3 out_normal;
in vec2 out_diffuse_texcoord;
in vec2 out_lightmap_texcoord;

out vec4 fragment;

void main() 
{
	vec4 lightmap_term = texture(lightmap_texture, out_lightmap_texcoord);
	lightmap_term.xyz *= lightmap_gamma;
	
	vec4 diffuse_term = texture(diffuse_texture, out_diffuse_texcoord);
	
	fragment = diffuse_term * lightmap_term;
	
	fragment.a *= alpha;
	
	if(should_test_alpha && fragment.a <= 0.25)
	{
	    discard;
	}
})";

	bsp_gpu_program_t::bsp_gpu_program_t() :
		gpu_program_t(vertex_shader_source, fragment_shader_source)
	{
		location_location = gpu.get_attribute_location(get_id(), "location");
		diffuse_texcoord_location = gpu.get_attribute_location(get_id(), "diffuse_texcoord");
		lightmap_texcoord_location = gpu.get_attribute_location(get_id(), "lightmap_texcoord");
	}

	void bsp_gpu_program_t::on_bind()
	{
		gpu.enable_vertex_attribute_array(location_location);
		gpu.enable_vertex_attribute_array(diffuse_texcoord_location);
		gpu.enable_vertex_attribute_array(lightmap_texcoord_location);

		gpu.set_vertex_attrib_pointer(location_location, 3, gpu_data_type_e::FLOAT, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, location)));
		gpu.set_vertex_attrib_pointer(diffuse_texcoord_location, 2, gpu_data_type_e::FLOAT, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, diffuse_texcoord)));
		gpu.set_vertex_attrib_pointer(lightmap_texcoord_location, 2, gpu_data_type_e::FLOAT, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, lightmap_texcoord)));
	}

	void bsp_gpu_program_t::on_unbind()
	{
		gpu.disable_vertex_attribute_array(location_location);
		gpu.disable_vertex_attribute_array(diffuse_texcoord_location);
		gpu.disable_vertex_attribute_array(lightmap_texcoord_location);
	}
}
