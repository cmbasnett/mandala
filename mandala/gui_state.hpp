#pragma once

//mandala
#include "state.hpp"
#include "gui_layout.hpp"

namespace mandala
{
	struct input_event_t;

	struct gui_state_t : state_t
	{
		gui_state_t();
		virtual ~gui_state_t();

		virtual void render() override;

		std::shared_ptr<gui_layout_t> layout;
	};
};