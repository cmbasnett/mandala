#pragma once

//mandala
#include "../gui_state.hpp"
#include "../gui_label.hpp"

namespace mandala
{
	namespace armada
	{
		struct console_state_t : gui_state_t
		{
			console_state_t();

			std::shared_ptr<gui_node_t> root_node;
			std::shared_ptr<gui_label_t> output_label;
			std::shared_ptr<gui_label_t> input_label;
		};
	}
}
