//std
#include <sstream>

//glm
#include <glm\ext.hpp>

//mandala
#include "../app.hpp"
#include "../platform.hpp"
#include "../gui_label.hpp"
#include "../gui_scroll.hpp"
#include "../gui_image.hpp"
#include "../gui_button.hpp"
#include "../bitmap_font.hpp"

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

            paused_label = std::make_shared<gui_label_t>();
            paused_label->set_bitmap_font(app.resources.get<bitmap_font_t>(hash_t("terminal_16.fnt")));
            paused_label->set_string(L"PAUSED");
            paused_label->set_vertical_alignment(gui_label_t::vertical_alignment_e::middle);
            paused_label->set_justification(gui_label_t::justification_e::center);
            paused_label->set_dock_mode(gui_dock_mode_e::fill);

            scroll->adopt(paused_label);

            for (auto i = 0; i < 50; ++i)
            {
                auto a = std::make_shared<gui_image_t>();
                a->set_sprite(sprite_ref_t(hash_t("white.tpsb"), hash_t("white.png")));
                a->set_is_autosized_to_texture(false);
                a->set_size(vec2_t(1, 0));
                a->set_color(vec4_t(static_cast<float32_t>(i) / 50, 0, 0, 1));
                a->set_dock_mode(gui_dock_mode_e::left);

                scroll->adopt(a);
            }

            auto button = std::make_shared<gui_button_t>();
            button->set_size(vec2_t(100, 100));
            button->set_anchor_flags(gui_anchor_flag_all);
            button->_hover_function = [&]()
            {
                std::cout << "hover" << std::endl;
            };

            button->_unhover_function = [&]()
            {
                std::cout << "unhover" << std::endl;
            };

            button->_pressed_function = [&]()
            {
                std::cout << "pressed" << std::endl;
            };

            layout->adopt(button);

            layout->adopt(scroll);
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
