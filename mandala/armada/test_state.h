#pragma once

#include "../state.h"

namespace mandala
{
	namespace armada
	{
		struct test_state_t : state_t
		{
			uint64_t index = 0;

			virtual void on_input_event(input_event_t& input_event);
			virtual void on_active();
			virtual void on_passive();
			virtual void on_enter();
			virtual void on_exit();
			virtual void on_stop_input();
			virtual void on_start_input();

			test_state_t(uint64_t index);
		};
	};
};
