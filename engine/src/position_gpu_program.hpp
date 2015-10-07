#pragma once

#include "gpu_program.hpp"
#include "glm.hpp"

using namespace glm;

namespace mandala
{
    struct position_gpu_program : gpu_program
    {
        struct vertex
        {
            vertex() = default;
            vertex(vec3 location)
            {
this->location = location;            }

            vec3 location;
        };

        typedef vertex vertex_type;

        position_gpu_program() :
            gpu_program(R"(#version 400

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;

in vec3 location;

out vec4 out_color;

void main() 
{
    gl_Position = view_projection_matrix * (world_matrix * vec4(location, 1));

    out_color = color;
})", R"(#version 400

uniform vec4 color;

in vec4 out_color;

out vec4 fragment;

void main() 
{
    fragment = out_color * color;
}  )")
        {
            location_location = gpu.get_attribute_location(get_id(), "location");
        }

        void on_bind() override
        {
            gpu.enable_vertex_attribute_array(location_location);
            gpu.set_vertex_attrib_pointer(location_location, sizeof(vec3) / sizeof(vec3::value_type), gpu_data_type_<vec3::value_type>::VALUE, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, location)));
        }

        void on_unbind() override
        {
            gpu.disable_vertex_attribute_array(location_location);
        }


    private:
       gpu_location location_location;
    };
}
