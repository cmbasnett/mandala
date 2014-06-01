#pragma once

//mandala
#include "../gui_state.hpp"
#include "../gui/label.hpp"

namespace mandala
{
	namespace armada
	{
		struct pause_state_t : gui_state_t
		{
			std::shared_ptr<gui::label_t> paused_label;
			std::vector<std::shared_ptr<bitmap_font_t>> bitmap_fonts;
			size_t bitmap_font_index = 0;

			pause_state_t();
			virtual ~pause_state_t();

			virtual void on_active() override;
			virtual void on_input_event(input_event_t& input_event) override;
		};
	};
};
