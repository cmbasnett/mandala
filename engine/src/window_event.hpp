#pragma once

#if defined(NAGA_PC)
//naga
#include "types.hpp"
#include "rectangle.hpp"

namespace naga
{
    enum class WindowEventType : u8
    {
        RESIZE,
        MOVE
    };

    struct WindowEvent
    {
        typedef details::Rectangle<u16> RectangleType;

		WindowEventType type;
		RectangleType rectangle;
    };
}
#endif
