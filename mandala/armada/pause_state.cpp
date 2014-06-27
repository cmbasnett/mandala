//std
#include <sstream>

//glm
#include <glm\ext.hpp>

//mandala
#include "../app.hpp"
#include "../platform.hpp"
#include "../gui/label.hpp"
#include "../gui/gui_image.hpp"
#include "../bitmap_font.hpp"

//armada
#include "pause_state.hpp"

namespace mandala
{
	namespace armada
    {
		pause_state_t::pause_state_t()
        {
			link_flags = state_t::link_flag_render;

			bitmap_fonts.push_back(app.resources.get<bitmap_font_t>(hash_t("04b_19_32_s.fnt")));

			paused_label = std::make_shared<gui::label_t>();
			paused_label->bitmap_font = bitmap_fonts[bitmap_font_index];
			paused_label->string = L"PAUSED";
			paused_label->vertical_alignment = gui::label_t::vertical_alignment_t::middle;
			paused_label->justification = gui::label_t::justification_t::center;
			paused_label->size = vec2_t(200, 100);
			paused_label->anchor_flags = gui::node_t::anchor_flag_horizontal | gui::node_t::anchor_flag_vertical;
			paused_label->should_use_gradient = true;
			paused_label->gradient.color1 = vec4_t(1, 0, 1, 1);
			paused_label->gradient.color2 = vec4_t(0, 1, 0, 1);

			layout->adopt(paused_label);

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
            if (input_event.device_type == input_event_t::device_type_t::keyboard &&
                input_event.keyboard.key == input_event_t::keyboard_t::key_t::escape &&
				input_event.keyboard.type == input_event_t::keyboard_t::type_t::key_press)
            {
				app.states.pop(shared_from_this());

				input_event.is_consumed = true;

				return;
			}

			if (input_event.device_type == input_event_t::device_type_t::keyboard &&
				input_event.keyboard.key == input_event_t::keyboard_t::key_t::backspace &&
				(input_event.keyboard.type == input_event_t::keyboard_t::type_t::key_press || input_event.keyboard.type == input_event_t::keyboard_t::type_t::key_repeat))
			{
				if (!paused_label->string.empty())
				{
					paused_label->string.resize(paused_label->string.size() - 1);
					paused_label->clean();

					input_event.is_consumed = true;

					return;
				}
			}

			if (input_event.device_type == input_event_t::device_type_t::keyboard &&
				(input_event.keyboard.key == input_event_t::keyboard_t::key_t::enter || input_event.keyboard.key == input_event_t::keyboard_t::key_t::kp_enter) &&
				(input_event.keyboard.type == input_event_t::keyboard_t::type_t::key_press || input_event.keyboard.type == input_event_t::keyboard_t::type_t::key_repeat))
			{
				paused_label->string += L"\n";
				paused_label->clean();

				input_event.is_consumed = true;

				return;
			}

			if (input_event.keyboard.type == input_event_t::keyboard_t::type_t::character)
			{
				paused_label->string += input_event.keyboard.character;
				paused_label->clean();

				input_event.is_consumed = true;

				return;
			}

			if (input_event.device_type == input_event_t::device_type_t::touch &&
				input_event.touch.type == input_event_t::touch_t::type_t::button_press)
			{
				gui::node_t::trace_args_t trace_args;
				trace_args.circle.origin = input_event.touch.position;
				trace_args.circle.radius = 8.0f;

				gui::node_t::trace_result_t trace_result;

				auto did_hit = gui::node_t::trace(layout, trace_args, trace_result);

				std::cout << trace_result.nodes_hit.size() << std::endl;

				for (auto& node : trace_result.nodes_hit)
				{
					node->on_input_event(input_event);

					if (input_event.is_consumed)
					{
						break;
					}
				}
			}
			else if (input_event.device_type == input_event_t::device_type_t::touch &&
				input_event.touch.type == input_event_t::touch_t::type_t::scroll)
			{
				if (input_event.touch.position_delta.y > 0)
				{
					++bitmap_font_index;
				}
				else if (input_event.touch.position_delta.y < 0)
				{
					--bitmap_font_index;
				}

				paused_label->bitmap_font = bitmap_fonts[bitmap_font_index % bitmap_fonts.size()];
				paused_label->clean();
			}
        }
	};
};
