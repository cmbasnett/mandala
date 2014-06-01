#pragma once

//mandala
#include "mandala.hpp"

//std
#include <memory>

namespace mandala
{
	struct input_event_t;
	struct state_mgr_t;

	struct state_t : std::enable_shared_from_this<state_t>
	{
		typedef uint8_t link_flags_t;

		enum : link_flags_t
		{
			link_flag_none =	(0 << 0),
			link_flag_render =  (1 << 0),
			link_flag_input =   (1 << 1),
			link_flag_tick =    (1 << 2),
			link_flag_all =     (link_flag_render | link_flag_input | link_flag_tick)
		};

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

		link_flags_t link_flags;
		std::weak_ptr<state_t> next_state;
		std::weak_ptr<state_t> previous_state;
	};
};
