#pragma once

#if defined(MANDALA_PC)
//mandala
#include "types.hpp"
#include "rectangle.hpp"

namespace mandala
{
    struct window_event_t
    {
        typedef rectangle_u16 rectangleype;

        enum class type_e : u8
        {
            RESIZE,
            MOVE
        };

        type_e type;
        rectangleype rectangle;
    };
}
#endif
