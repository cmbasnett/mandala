//mandala
#include "bitmap_font_gpu_program.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
    std::string bitmap_font_gpu_program_t::vertex_shader_source = R"(
#version 400

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
#version 400

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
		location_location = gpu.get_attribute_location(get_id(), "location");
		texcoord_location = gpu.get_attribute_location(get_id(), "texcoord");
	}

	void bitmap_font_gpu_program_t::on_bind()
	{
		gpu.enable_vertex_attribute_array(location_location);
		gpu.enable_vertex_attribute_array(texcoord_location);

		gpu.set_vertex_attrib_pointer(location_location, 2, gpu_data_type_e::FLOAT, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, location)));
		gpu.set_vertex_attrib_pointer(texcoord_location, 2, gpu_data_type_e::FLOAT, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, texcoord)));
	}

	void bitmap_font_gpu_program_t::on_unbind()
	{
		gpu.disable_vertex_attribute_array(location_location);
		gpu.disable_vertex_attribute_array(texcoord_location);
	}
}