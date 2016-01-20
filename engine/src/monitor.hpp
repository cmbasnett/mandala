#pragma once

//mandala
#include "rectangle.hpp"

namespace naga
{
    struct monitor
    {
        const char* name = nullptr;
        vec2_i32 physical_size;
        vec2_i32 position;
    };
}