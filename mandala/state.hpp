#pragma once

//mandala
#include "types.hpp"

//std
#include <memory>

namespace mandala
{
	struct input_event_t;
	struct state_mgr_t;

	struct state_t : std::enable_shared_from_this<state_t>
	{
		virtual void tick(float32_t dt) { }
		virtual void render() { }
		virtual void on_input_event(input_event_t& input_event) { }
		virtual void on_active() { }
		virtual void on_passive() { }
		virtual void on_enter() { }
		virtual void on_exit() { }
		virtual void on_stop_tick() { }
		virtual void on_start_tick() { }
		virtual void on_stop_render() { }
		virtual void on_start_render() { }
		virtual void on_stop_input() { }
		virtual void on_start_input() { }
	};
};
