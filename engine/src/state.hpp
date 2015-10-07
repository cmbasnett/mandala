#pragma once

//mandala
#include "platform_defs.hpp"
#include "types.hpp"

#if defined(MANDALA_PC)
#include "window_event.hpp"
#endif

//boost
#include <boost\enable_shared_from_this.hpp>

namespace mandala
{
    struct input_event_t;
    struct state_mgr;
    struct gui_layout;

    struct state : public boost::enable_shared_from_this<state>
    {
        state();

        virtual void tick(f32 dt);
        virtual void render();
        virtual bool on_input_event(input_event_t& input_event);
#if defined(MANDALA_PC)
        virtual void on_window_event(window_event& window_event);
#endif
        virtual void on_active() { }
        virtual void on_passive() { }
        virtual void on_enter();
        virtual void on_exit() { }
        virtual void on_stop_tick() { }
        virtual void on_start_tick() { }
        virtual void on_stop_render() { }
        virtual void on_start_render() { }
        virtual void on_stop_input() { }
        virtual void on_start_input() { }
        virtual void on_tick(f32 dt) { }

        const boost::shared_ptr<gui_layout>& get_layout() const { return layout; }

    private:
        boost::shared_ptr<gui_layout> layout;
    };
}
