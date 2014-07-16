#pragma once

//mandala
#include "gui_node.hpp"
#include "frame_buffer.hpp"

namespace mandala
{
    struct gui_canvas_t : gui_node_t
    {
        frame_buffer_t frame_buffer;
    };
}