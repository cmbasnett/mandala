#pragma once

#include "types.hpp"

namespace mandala
{
    struct camera_params
    {
        mat4 view_matrix;
        mat4 projection_matrix;
        vec3 location;
        frustum_f32 frustum;
    };
}