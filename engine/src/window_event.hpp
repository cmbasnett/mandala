#pragma once

#if defined(NAGA_PC)
//naga
#include "types.hpp"
#include "rectangle.hpp"

namespace naga
{
    enum class window_event_type : u8
    {
        RESIZE,
        MOVE
    };

    struct window_event
    {
        typedef rectangle_u16 rectangle_type;

        window_event_type type;
        rectangle_type rectangle;
    };
}
#endif
