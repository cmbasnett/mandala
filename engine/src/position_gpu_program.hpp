#pragma once

#include "gpu_program.hpp"
#include "glm.hpp"

using namespace glm;

namespace naga
{
    struct position_gpu_program : GpuProgram
    {
        struct Vertex
        {
            Vertex() = default;
            Vertex(vec3 location)
            {
this->location = location;            }

            vec3 location;
        };

        typedef Vertex VertexType;

        position_gpu_program() :
            GpuProgram(R"(#version 400

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
            gpu.set_vertex_attrib_pointer(location_location, sizeof(vec3) / sizeof(vec3::value_type), GpuDataType<vec3::value_type>::VALUE, false, sizeof(VertexType), reinterpret_cast<void*>(offsetof(VertexType, location)));
        }

        void on_unbind() override
        {
            gpu.disable_vertex_attribute_array(location_location);
        }


    private:
       GpuLocation location_location;
    };
}
