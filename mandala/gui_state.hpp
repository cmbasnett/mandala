#pragma once

//mandala
#include "state.hpp"
#include "layout.hpp"

namespace mandala
{
	struct input_event_t;

	struct gui_state_t : state_t
	{
		gui_state_t();
		virtual ~gui_state_t();

		virtual void render() override;

		std::shared_ptr<gui::layout_t> layout;
	};
};
