#pragma once

//mandala
#include "../gui_state.hpp"
#include "../gui_label.hpp"

namespace mandala
{
	struct debug_state_t : gui_state_t
	{
		debug_state_t();

		boost::shared_ptr<gui_label_t> label;

		virtual void tick(float32_t dt) override;
	};
}