#pragma once

//mandala
#include "rectangle.hpp"

namespace mandala
{
    struct monitor_t
    {
        const char* name = nullptr;
        vec2_i32 physical_size;
        vec2_i32 position;
    };
}