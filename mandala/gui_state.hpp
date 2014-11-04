#pragma once

//mandala
#include "state.hpp"
#include "gui_layout.hpp"

#if defined(_WIN32) || defined(WIN32)
#include "window_event.hpp"
#endif

namespace mandala
{
	struct input_event_t;

	struct gui_state_t : state_t
	{
		gui_state_t();
		virtual ~gui_state_t();

		virtual void tick(float32_t dt) override;
		virtual void render() override;
        virtual void on_input_event(input_event_t& input_event) override;
#if defined(_WIN32) || defined(WIN32)
        virtual void on_window_event(window_event_t& window_event) override;
#endif

		std::shared_ptr<gui_layout_t> layout;
	};
}
