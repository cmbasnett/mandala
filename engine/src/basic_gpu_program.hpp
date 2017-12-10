#pragma once

#include "gpu_program.hpp"
#include "glm.hpp"

using namespace glm;

namespace naga
{
    struct basic_gpu_program : GpuProgram
    {
        struct Vertex
        {
            Vertex() = default;
            Vertex(vec3 location, vec4 color)
            {
                this->location = location;
                this->color = color;
            }

            vec3 location;
            vec4 color;
        };

        typedef Vertex VertexType;

        basic_gpu_program() :
            GpuProgram(R"(#version 400

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;
uniform float texture_scale;

in vec3 location;
in vec4 color;

out vec4 out_color;
out vec2 out_texcoord;

void main()
{
    gl_Position = view_projection_matrix * (world_matrix * vec4(location, 1));

    out_texcoord.x = location.x * texture_scale;
    out_texcoord.y = location.z * texture_scale;

    out_color = color;
}
)", R"(#version 400

uniform sampler2D diffuse_texture;
uniform vec4 color;

in vec4 out_color;
in vec2 out_texcoord;

out vec4 fragment;

void main()
{
    fragment = texture2D(diffuse_texture, out_texcoord) * out_color * color;
}
)")
        {
            location_location = gpu.get_attribute_location(get_id(), "location");
            color_location = gpu.get_attribute_location(get_id(), "color");
        }

        void on_bind() override
        {
            gpu.enable_vertex_attribute_array(location_location);
            gpu.enable_vertex_attribute_array(color_location);
            gpu.set_vertex_attrib_pointer(location_location, sizeof(vec3) / sizeof(vec3::value_type), GpuDataType<vec3::value_type>::VALUE, false, sizeof(VertexType), reinterpret_cast<void*>(offsetof(VertexType, location)));
            gpu.set_vertex_attrib_pointer(color_location, sizeof(vec4) / sizeof(vec4::value_type), GpuDataType<vec4::value_type>::VALUE, false, sizeof(VertexType), reinterpret_cast<void*>(offsetof(VertexType, color)));
        }

        void on_unbind() override
        {
            gpu.disable_vertex_attribute_array(location_location);
            gpu.disable_vertex_attribute_array(color_location);
        }


    private:
       GpuLocation location_location;
       GpuLocation color_location;
    };
}
