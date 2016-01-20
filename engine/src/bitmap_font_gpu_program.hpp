#pragma once

#include "gpu_program.hpp"
#include "glm.hpp"

using namespace glm;

namespace naga
{
    struct bitmap_font_gpu_program : gpu_program
    {
        struct vertex
        {
            vertex() = default;
            vertex(vec2 location, vec2 texcoord)
            {
this->location = location;
this->texcoord = texcoord;            }

            vec2 location;
            vec2 texcoord;
        };

        typedef vertex vertex_type;

        bitmap_font_gpu_program() :
            gpu_program(R"(#version 400

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
})", R"(#version 400

uniform sampler2D diffuse_texture;

in vec2 out_texcoord;
in vec4 out_color;

void main() 
{
    gl_FragColor = texture2D(diffuse_texture, out_texcoord) * out_color;
})")
        {
            location_location = gpu.get_attribute_location(get_id(), "location");
            texcoord_location = gpu.get_attribute_location(get_id(), "texcoord");
        }

        void on_bind() override
        {
            gpu.enable_vertex_attribute_array(location_location);
            gpu.enable_vertex_attribute_array(texcoord_location);
            gpu.set_vertex_attrib_pointer(location_location, sizeof(vec2) / sizeof(vec2::value_type), gpu_data_type_<vec2::value_type>::VALUE, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, location)));
            gpu.set_vertex_attrib_pointer(texcoord_location, sizeof(vec2) / sizeof(vec2::value_type), gpu_data_type_<vec2::value_type>::VALUE, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, texcoord)));
        }

        void on_unbind() override
        {
            gpu.disable_vertex_attribute_array(location_location);
            gpu.disable_vertex_attribute_array(texcoord_location);
        }


    private:
       gpu_location location_location;
       gpu_location texcoord_location;
    };
}
