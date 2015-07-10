//mandala
#include "basic_gpu_program.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
    std::string basic_gpu_program_t::vertex_shader_source = R"(
#version 400

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;

in vec3 location;
in vec4 color;

out vec4 out_color;

void main() 
{
    gl_Position = view_projection_matrix * (world_matrix * vec4(location, 1));

    out_color = color;
})";

    std::string basic_gpu_program_t::fragment_shader_source = R"(
#version 400

uniform vec4 color;

in vec4 out_color;

out vec4 fragment;

void main() 
{
    fragment = out_color * color;
})";

    basic_gpu_program_t::basic_gpu_program_t() :
        gpu_program_t(vertex_shader_source, fragment_shader_source)
    {
        location_location = gpu.get_attribute_location(get_id(), "location");
        color_location = gpu.get_attribute_location(get_id(), "color");
    }

    void basic_gpu_program_t::on_bind()
    {
        gpu.enable_vertex_attribute_array(location_location);
        gpu.enable_vertex_attribute_array(color_location);

        gpu.set_vertex_attrib_pointer(location_location, 3, gpu_data_type_e::FLOAT, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, location)));
        gpu.set_vertex_attrib_pointer(color_location, 4, gpu_data_type_e::FLOAT, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, color)));
    }

    void basic_gpu_program_t::on_unbind()
    {
        gpu.disable_vertex_attribute_array(location_location);
        gpu.disable_vertex_attribute_array(color_location);
    }
}
