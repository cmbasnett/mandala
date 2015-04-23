#pragma once

//mandala
#include "../gui_state.hpp"
#include "../gui_label.hpp"

namespace mandala
{
	namespace armada
	{
		struct pause_state_t : gui_state_t
		{
			boost::shared_ptr<gui_label_t> paused_label;

			pause_state_t();

			virtual void on_active() override;
			virtual void on_input_event(input_event_t& input_event) override;
		};
	};
};
