#pragma once

#include "gpu_program.hpp"
#include "glm.hpp"

using namespace glm;

namespace naga
{
    struct gui_image_gpu_program : gpu_program
    {
        struct vertex
        {
            vertex() = default;
            vertex(vec3 location, vec2 texcoord)
            {
this->location = location;
this->texcoord = texcoord;            }

            vec3 location;
            vec2 texcoord;
        };

        typedef vertex vertex_type;

        gui_image_gpu_program() :
            gpu_program(R"(#version 400

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;

in vec3 location;
in vec2 texcoord;

out vec2 out_texcoord;

void main() 
{
    out_texcoord = texcoord;
    
    gl_Position = (view_projection_matrix) * (world_matrix * vec4(location, 1));
})", R"(#version 400

uniform sampler2D diffuse_texture;
uniform vec4 color;
uniform vec2 region_min;
uniform vec2 region_max;
uniform vec2 frame_min;
uniform vec2 frame_max;
uniform vec2 texcoord_origin;
uniform vec2 texcoord_scale;
uniform float rotation;

in vec2 out_texcoord;

out vec4 fragment;

void main()
{
    // rotation
    mat2 rotation_matrix = mat2(cos(rotation), -sin(rotation), sin(rotation), cos(rotation));
    vec2 texcoord = out_texcoord * texcoord_scale;
    texcoord -= texcoord_origin;
    texcoord = rotation_matrix * texcoord;
    texcoord += texcoord_origin;

    // tiling
    texcoord.x = mod(texcoord.x, 1.0);
    texcoord.y = mod(texcoord.y, 1.0);

    // move texcoord from frame space to texture space
    texcoord = frame_min + (texcoord * (frame_max - frame_min));

    // if texcoord outside of region, discard fragment
    if (texcoord.x < region_min.x ||
        texcoord.x > region_max.x ||
        texcoord.y < region_min.y ||
        texcoord.y > region_max.y)
    {
        discard;
    }

    fragment = texture2D(diffuse_texture, texcoord) * color;
})")
        {
            location_location = gpu.get_attribute_location(get_id(), "location");
            texcoord_location = gpu.get_attribute_location(get_id(), "texcoord");
        }

        void on_bind() override
        {
            gpu.enable_vertex_attribute_array(location_location);
            gpu.enable_vertex_attribute_array(texcoord_location);
            gpu.set_vertex_attrib_pointer(location_location, sizeof(vec3) / sizeof(vec3::value_type), gpu_data_type_<vec3::value_type>::VALUE, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, location)));
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
