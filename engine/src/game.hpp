#pragma once

//naga
#include "types.hpp"
#include "platform_defs.hpp"

namespace naga
{
    struct InputEvent;
#if defined(NAGA_PC)
    struct WindowEvent;
#endif

    struct Game
    {
        virtual void on_run_start() = 0;
        virtual void on_run_end() = 0;
        virtual void on_tick_start(f32) = 0;
        virtual void on_tick_end(f32) = 0;
        virtual void on_render_start() = 0;
        virtual void on_render_end() = 0;
		virtual bool on_input_event(InputEvent& input_event) = 0;
#if defined(NAGA_PC)
		virtual void on_window_event(WindowEvent& window_event) = 0;
#endif
    };
}