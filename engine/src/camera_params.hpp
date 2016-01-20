#pragma once

#include "types.hpp"
#include "frustum.hpp"

namespace naga
{
    struct camera_params
    {
        mat4 view_matrix;
        mat4 projection_matrix;
        vec3 location;
        frustum frustum;
    };
}