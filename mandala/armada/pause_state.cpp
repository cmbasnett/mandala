//std
#include <sstream>

//glm
#include <glm\ext.hpp>

//mandala
#include "../app.hpp"
#include "../platform.hpp"
#include "../gui_label.hpp"
#include "../bitmap_font.hpp"

//armada
#include "pause_state.hpp"

namespace mandala
{
	namespace armada
    {
		pause_state_t::pause_state_t()
        {
			paused_label = std::make_shared<gui_label_t>();
            paused_label->set_bitmap_font(app.resources.get<bitmap_font_t>(hash_t("terminal_16.fnt")));
            paused_label->set_string(L"PAUSED");
            paused_label->set_vertical_alignment(gui_label_t::vertical_alignment_e::middle);
            paused_label->set_justification(gui_label_t::justification_e::center);
            paused_label->set_dock_mode(gui_dock_mode_e::fill);

            layout->adopt(paused_label);
        }

        pause_state_t::~pause_state_t()
        {
        }

		void pause_state_t::on_active()
		{
		}

		void pause_state_t::on_input_event(input_event_t& input_event)
		{
			gui_state_t::on_input_event(input_event);

			if (input_event.is_consumed)
			{
				return;
			}

            if (input_event.device_type == input_event_t::device_type_e::keyboard &&
                input_event.keyboard.key == input_event_t::keyboard_t::key_e::escape &&
                input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_press)
            {
                app.states.pop(shared_from_this());

                input_event.is_consumed = true;
            }
        }
	};
};
