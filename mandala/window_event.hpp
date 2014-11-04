#pragma once

#if defined(_WIN32) || defined(WIN32)

//mandala
#include "types.hpp"

namespace mandala
{
    struct window_event_t
    {
        enum class type_e : uint8_t
        {
            resize
        };

        type_e type;
        uint16_t x = 0;
        uint16_t y = 0;
        uint16_t width = 0;
        uint16_t height = 0;
    };
}
#endif
