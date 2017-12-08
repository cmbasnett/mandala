#pragma once

//naga
#include "platform_defs.hpp"
#include "types.hpp"

#if defined(NAGA_PC)
#include "window_event.hpp"
#endif

//boost
#include <boost\enable_shared_from_this.hpp>

namespace naga
{
	struct InputEvent;
	struct GUILayout;

	struct State : public boost::enable_shared_from_this<State>
    {
		State();

        virtual void tick(f32 dt);
        virtual void render();
        virtual bool on_input_event(InputEvent& input_event);
#if defined(NAGA_PC)
        virtual void on_window_event(WindowEvent& window_event);
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

		const boost::shared_ptr<GUILayout>& get_layout() const { return layout; }

    private:
        boost::shared_ptr<GUILayout> layout;
    };
}
