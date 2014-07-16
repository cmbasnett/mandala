#pragma once

//mandala
#include "input_event.hpp"

//std
#include <vector>
#include <memory>
#include <deque>

namespace mandala
{
	struct app_t;
	struct state_t;

	struct state_mgr_t
	{
		struct operation_t
		{
			enum class type_e : uint8_t
			{
				push,
				pop
			};

            type_e type = type_e::push;
			std::shared_ptr<state_t> state;
		};

		state_mgr_t();

		void tick(float32_t dt);
		void render();
		void on_input_event(input_event_t& input_event);

		void push(const std::shared_ptr<state_t>& state);
		void pop(const std::shared_ptr<state_t>& state);
		void purge();
		
	private:
		state_mgr_t(const state_mgr_t&) = delete;
		state_mgr_t& operator=(const state_mgr_t&) = delete;

		std::deque<operation_t> operation_queue;
		std::vector<std::shared_ptr<state_t>> states;
	};
};
