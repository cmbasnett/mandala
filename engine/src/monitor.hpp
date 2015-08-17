#pragma once

#include "rectangle.hpp"

namespace mandala
{
    struct monitor_t
    {
        const char* name = nullptr;
        vec2_i32_t physical_size;
        vec2_i32_t position;
    };
}