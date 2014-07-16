#pragma once

#include "types.hpp"

#define GENERATE_ATTRIBUTE(RET,NAM,...) RET _##NAM;

namespace mandala
{
    struct gpu_vertex_t
    {
        GENERATE_ATTRIBUTE(vec3_t, position);
        GENERATE_ATTRIBUTE(vec2_t, texcoord);

        gpu_vertex_t()
        {
        }
    };
}