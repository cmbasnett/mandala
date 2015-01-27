#pragma once

#if defined(MANDALA_PC)
//mandala
#include "types.hpp"
#include "rectangle.hpp"

namespace mandala
{
    struct window_event_t
    {
        typedef rectangle_u16_t rectangle_type;

        enum class type_e : uint8_t
        {
            resize,
            move
        };

        type_e type;
        rectangle_type rectangle;
    };
}
#endif
