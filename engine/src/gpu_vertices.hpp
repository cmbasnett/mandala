#pragma once

//mandala
#include "types.hpp"

namespace mandala
{
    struct basic_gpu_vertex_t
    {
        typedef vec3_t location_type;
        typedef rgba_type color_type;
        typedef vec2_t texcoord_type;

        basic_gpu_vertex_t() = default;
        basic_gpu_vertex_t(const location_type& location, const color_type& color, const texcoord_type& texcoord) :
            location(location),
            color(color),
            texcoord(texcoord)
        {
        }

        location_type location;
        color_type color;
        texcoord_type texcoord;
    };

    struct model_vertex_t
    {
        typedef vec3_t location_type;
        typedef vec3_t normal_type;
        typedef vec2_t texcoord_type;
        typedef vec4_i32_t bone_indices_type;
        typedef vec4_t bone_weights_type;

        location_type location;
        normal_type normal;
        normal_type tangent;
        texcoord_type texcoord;
        bone_indices_type bone_indices_0;
        bone_indices_type bone_indices_1;
        bone_weights_type bone_weights_0;
        bone_weights_type bone_weights_1;
    };
}
