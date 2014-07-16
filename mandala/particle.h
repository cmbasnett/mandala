#pragma once

//mandala
#include "types.hpp"

namespace mandala
{
    struct particle_t
    {
        vec3_t position;
        vec3_t velocity;
        vec3_t acceleration;
        vec2_t scale;
    };
}
