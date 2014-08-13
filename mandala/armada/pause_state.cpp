﻿//std
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
			raw_label->bitmap_font = bitmap_fonts[bitmap_font_index];
			raw_label->string = L"PAUSED";
			raw_label->vertical_alignment = gui_label_t::vertical_alignment_e::middle;
			raw_label->justification = gui_label_t::justification_e::center;
			raw_label->size = vec2_t(200, 100);
			raw_label->anchor_flags = gui_anchor_flag_left | gui_anchor_flag_bottom;
			raw_label->should_use_color_codes = false;

			color_label = std::make_shared<gui_label_t>();
			color_label->bitmap_font = bitmap_fonts[bitmap_font_index];
			color_label->string = L"PAUSED";
			color_label->vertical_alignment = gui_label_t::vertical_alignment_e::middle;
			color_label->justification = gui_label_t::justification_e::center;
			color_label->size = vec2_t(200, 100);
			color_label->anchor_flags = gui_anchor_flag_right | gui_anchor_flag_bottom;

			layout->adopt(raw_label);
			layout->adopt(color_label);

			layout->clean();
        }

        pause_state_t::~pause_state_t()
        {
        }

		void pause_state_t::on_active()
		{
		}

		void pause_state_t::on_input_event(input_event_t& input_event)
        {
			bool did_string_change = false;
			auto& string = raw_label->string;

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

				if (input_event.device_type == input_event_t::device_type_e::touch &&
					input_event.touch.type == input_event_t::touch_t::type_e::button_press)
				{
					gui_node_t::trace_args_t trace_args;
					trace_args.circle.origin = input_event.touch.position;
					trace_args.circle.radius = 8.0f;

					gui_node_t::trace_result_t trace_result;

					auto did_hit = gui_node_t::trace(layout, trace_args, trace_result);

					std::cout << trace_result.nodes_hit.size() << std::endl;

					for (auto& node : trace_result.nodes_hit)
					{
						node->on_input_event(input_event);

						if (input_event.is_consumed)
						{
							break;
						}
					}

					if (input_event.is_consumed)
					{
						break;
					}
				}

				break;
			}

			if (did_string_change)
			{
				color_label->string = string;

				raw_label->clean();
				color_label->clean();
			}
        }
	};
};
