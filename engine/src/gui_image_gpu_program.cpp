//mandala
#include "gui_image_gpu_program.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
	std::string gui_image_gpu_program_t::vertex_shader_source = R"(
#version 400

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

    std::string gui_image_gpu_program_t::fragment_shader_source = R"(
#version 400

uniform sampler2D diffuse_texture;
uniform vec4 color;
uniform vec2 region_min;
uniform vec2 region_size;

in vec2 out_texcoord;

out vec4 fragment;

void main()
{
    vec2 texcoord = region_min + mod(out_texcoord * region_size, region_size);

	fragment = texture2D(diffuse_texture, texcoord) * color;
}
)";

	gui_image_gpu_program_t::gui_image_gpu_program_t() :
		gpu_program_t(vertex_shader_source, fragment_shader_source)
	{
        location_location = gpu.get_attribute_location(get_id(), "location");
		texcoord_location = gpu.get_attribute_location(get_id(), "texcoord");
	}

	void gui_image_gpu_program_t::on_bind()
	{
		gpu.enable_vertex_attribute_array(location_location);
		gpu.enable_vertex_attribute_array(texcoord_location);

		gpu.set_vertex_attrib_pointer(location_location, 3, gpu_data_type_e::FLOAT, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, location)));
		gpu.set_vertex_attrib_pointer(texcoord_location, 2, gpu_data_type_e::FLOAT, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, texcoord)));
	}

	void gui_image_gpu_program_t::on_unbind()
	{
		gpu.disable_vertex_attribute_array(location_location);
		gpu.disable_vertex_attribute_array(texcoord_location);
	}
}
