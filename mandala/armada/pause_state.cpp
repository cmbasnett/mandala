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

//armada
#include "pause_state.hpp"

namespace mandala
{
	namespace armada
    {
		pause_state_t::pause_state_t()
        {
            auto scroll = std::make_shared<gui_scroll_t>();
            scroll->set_dock_mode(gui_dock_mode_e::fill);
			scroll->set_scroll_extents(aabb2_t(vec2_t(-512, 0), vec2_t(512, 0)));

			auto scroll2 = std::make_shared<gui_scroll_t>();
			scroll2->set_dock_mode(gui_dock_mode_e::left);
			scroll2->set_scroll_extents(aabb2_t(vec2_t(0, -384), vec2_t(0, 384)));
			scroll2->set_size(vec2_t(512, 0));

            paused_label = std::make_shared<gui_label_t>();
            paused_label->set_bitmap_font(resources.get<bitmap_font_t>(hash_t("unifont_14.fnt")));
			paused_label->set_string(strings.get(hash_t("replace_test")));
            paused_label->set_vertical_alignment(gui_label_t::vertical_alignment_e::middle);
            paused_label->set_justification(gui_label_t::justification_e::center);
            paused_label->set_dock_mode(gui_dock_mode_e::fill);
			paused_label->set_is_read_only(false);

            scroll->adopt(paused_label);

            auto button = std::make_shared<gui_button_t>();
            button->set_size(vec2_t(100, 100));
            button->set_anchor_flags(gui_anchor_flag_all);
            button->on_hover_function = [&]()
            {
                std::cout << "hover" << std::endl;
            };

            button->on_unhover_function = [&]()
            {
                std::cout << "unhover" << std::endl;
            };

            button->on_pressed_function = [&]()
            {
                std::cout << "pressed" << std::endl;
            };

            scroll->adopt(button);
			scroll->adopt(scroll2);
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
