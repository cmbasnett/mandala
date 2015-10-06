#pragma once

#include "types.hpp"

namespace mandala
{
    struct camera_params
    {
        mat4_t view_matrix;
        mat4_t projection_matrix;
        vec3_t location;
        frustum_f32_t frustum;
    };
}