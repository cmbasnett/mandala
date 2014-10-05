#pragma once

//mandala
#include "../gui_state.hpp"

namespace mandala
{
	struct gui_node_t;
	struct gui_label_t;
    struct gui_textfield_t;

	namespace armada
	{
		struct console_state_t : gui_state_t
		{
            typedef std::wstring command_string_type;

			console_state_t();

			virtual void on_input_event(input_event_t& input_event) override;

			std::shared_ptr<gui_node_t> root_node;
			std::shared_ptr<gui_label_t> output_label;
            std::shared_ptr<gui_textfield_t> input_textfield;

            static vec3_t echo_color;
            static vec3_t warning_color;
            static vec3_t error_color;
            
        private:
            std::deque<command_string_type> command_strings;
            std::deque<command_string_type>::iterator command_strings_itr = command_strings.end();
		};
	}
}
