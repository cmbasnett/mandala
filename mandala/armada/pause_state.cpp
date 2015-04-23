//std
#include <sstream>

//glm
#include <glm\ext.hpp>

//mandala
#include "../resource_mgr.hpp"
#include "../platform.hpp"
#include "../gui_label.hpp"
#include "../gui_scroll.hpp"
#include "../gui_image.hpp"
#include "../gui_button.hpp"
#include "../bitmap_font.hpp"
#include "../string_mgr.hpp"
#include "../state_mgr.hpp"
#include "../gui_element.hpp"

//armada
#include "pause_state.hpp"

namespace mandala
{
	namespace armada
    {
		pause_state_t::pause_state_t()
        {
            auto scroll = boost::make_shared<gui_scroll_t>();
            scroll->set_dock_mode(gui_dock_mode_e::fill);
            scroll->set_scroll_extents(aabb2_t(vec2_t(-512, 0), vec2_t(512, 0)));

            for (auto i = 0; i < 2; ++i)
            {
                auto e = boost::make_shared<gui_element_t>();

                scroll->adopt(e);
            }

            layout->adopt(scroll);
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
                states.pop(shared_from_this());

                input_event.is_consumed = true;
            }
        }
	};
};
