//std
#include <sstream>

//glm
#include <glm\ext.hpp>

//mandala
#include "../app.hpp"
#include "../platform.hpp"
#include "../gui_label.hpp"
#include "../gui_image.hpp"
#include "../bitmap_font.hpp"

//armada
#include "pause_state.hpp"

namespace mandala
{
	namespace armada
    {
		pause_state_t::pause_state_t()
        {
			bitmap_fonts.push_back(app.resources.get<bitmap_font_t>(hash_t("terminal_16.fnt")));

			raw_label = std::make_shared<gui_label_t>();
			raw_label->set_bitmap_font(bitmap_fonts[bitmap_font_index]);
			raw_label->set_string(L"PAUSED");
			raw_label->set_vertical_alignment(gui_label_t::vertical_alignment_e::middle);
			raw_label->set_justification(gui_label_t::justification_e::center);
			raw_label->set_size(vec2_t(200, 100));
			raw_label->set_anchor_flags(gui_anchor_flag_left | gui_anchor_flag_bottom);
			raw_label->set_should_use_color_codes(false);

			color_label = std::make_shared<gui_label_t>();
			color_label->set_bitmap_font(bitmap_fonts[bitmap_font_index]);
			color_label->set_string(L"PAUSED");
			color_label->set_vertical_alignment(gui_label_t::vertical_alignment_e::middle);
			color_label->set_justification(gui_label_t::justification_e::center);
			color_label->set_size(vec2_t(200, 100));
			color_label->set_anchor_flags(gui_anchor_flag_right | gui_anchor_flag_bottom);

			layout->adopt(raw_label);
			layout->adopt(color_label);
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

			bool did_string_change = false;
			auto string = raw_label->string();

			for (;;)
			{
				if (input_event.device_type == input_event_t::device_type_e::keyboard &&
					input_event.keyboard.key == input_event_t::keyboard_t::key_e::escape &&
					input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_press)
				{
					app.states.pop(shared_from_this());

					input_event.is_consumed = true;

					break;
				}

				if (input_event.device_type == input_event_t::device_type_e::keyboard &&
					input_event.keyboard.key == input_event_t::keyboard_t::key_e::backspace &&
					(input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_press || input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_repeat))
				{
					if (!string.empty())
					{
						string.resize(string.size() - 1);

						did_string_change = true;

						input_event.is_consumed = true;

						break;
					}
				}

				if (input_event.device_type == input_event_t::device_type_e::keyboard &&
					input_event.keyboard.key == input_event_t::keyboard_t::key_e::del &&
					(input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_press || input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_repeat))
				{
					auto link_flags = app.states.get_link_flags(shared_from_this());

					if ((link_flags & state_flag_render) == state_flag_render)
					{
						app.states.change_link_flags(shared_from_this(), link_flags & ~state_flag_render);
					}
					else
					{
						app.states.change_link_flags(shared_from_this(), link_flags | state_flag_render);
					}

					input_event.is_consumed = true;

					break;
				}

				if (input_event.device_type == input_event_t::device_type_e::keyboard &&
					(input_event.keyboard.key == input_event_t::keyboard_t::key_e::enter || input_event.keyboard.key == input_event_t::keyboard_t::key_e::kp_enter) &&
					(input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_press || input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_repeat))
				{
					string += L"\n";

					did_string_change = true;

					input_event.is_consumed = true;

					break;
				}

				if (input_event.keyboard.type == input_event_t::keyboard_t::type_e::character)
				{
					string += input_event.keyboard.character;

					did_string_change = true;

					input_event.is_consumed = true;

					break;
				}

				break;
			}

			if (did_string_change)
			{
				raw_label->set_string(string);
				color_label->set_string(string);
			}
        }
	};
};
