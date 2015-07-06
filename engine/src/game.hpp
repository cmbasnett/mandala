#pragma once

//mandala
#include "types.hpp"
#include "platform_defs.hpp"

namespace mandala
{
    struct input_event_t;
#if defined(MANDALA_PC)
    struct window_event_t;
#endif

    struct game_t
    {
        virtual void on_run_start() = 0;
        virtual void on_run_end() = 0;
        virtual void on_tick_start(float32_t) = 0;
        virtual void on_tick_end(float32_t) = 0;
        virtual void on_render_start() = 0;
        virtual void on_render_end() = 0;
        virtual bool on_input_event(input_event_t& input_event) = 0;
#if defined(MANDALA_PC)
        virtual void on_window_event(window_event_t& window_event) = 0;
#endif
    };
}