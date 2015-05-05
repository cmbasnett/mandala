#pragma once

//std
#include <string>

namespace mandala
{
	struct game_t
	{
        virtual void app_run_start() { }
        virtual void app_run_end() { }

        virtual void app_tick_start() { }
        virtual void app_tick_end() { }

        virtual void app_render_start() { }
        virtual void app_render_end() { }

        virtual void on_input_event(input_event_t& input_event) { }

        virtual std::string get_name() const { return "mandala"; }
	};
}