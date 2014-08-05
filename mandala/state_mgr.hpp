#pragma once

//mandala
#include "input_event.hpp"

//std
#include <vector>
#include <memory>
#include <deque>
#include <list>

namespace mandala
{
	struct app_t;
	struct state_t;

	struct state_mgr_t
	{
		typedef uint8_t link_flags_type;

		enum : link_flags_type
		{
			link_flag_none = (0 << 0),
			link_flag_render = (1 << 0),
			link_flag_input = (1 << 1),
			link_flag_tick = (1 << 2),
			link_flag_all = (link_flag_render | link_flag_input | link_flag_tick),
			link_flag_default = (link_flag_render)
		};

		struct node_t
		{
			std::shared_ptr<state_t> state;
			link_flags_type link_flags = link_flag_default;
		};

		struct operation_t
		{
			enum class type_e : uint8_t
			{
				push,
				pop
			};

            type_e type = type_e::push;
			std::shared_ptr<state_t> state;
			link_flags_type link_flags = link_flag_default;
		};

		state_mgr_t();

		void tick(float32_t dt);
		void render();
		void on_input_event(input_event_t& input_event);

		void push(const std::shared_ptr<state_t>& state, link_flags_type link_flags);
		void pop(const std::shared_ptr<state_t>& state);
		void purge();
		size_t count() const;
		
	private:
		state_mgr_t(const state_mgr_t&) = delete;
		state_mgr_t& operator=(const state_mgr_t&) = delete;

		std::list<node_t> nodes;
		std::deque<operation_t> operations;
	};
}
