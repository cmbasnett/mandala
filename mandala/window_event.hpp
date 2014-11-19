#pragma once

//mandala
#include "types.hpp"

#if defined(MANDALA_PC)

namespace mandala
{
    struct window_event_t
    {
        enum class type_e : uint8_t
        {
            resize,
			move
        };

        type_e type;
        uint16_t x = 0;
        uint16_t y = 0;
        uint16_t width = 0;
        uint16_t height = 0;
    };
}
#endif
