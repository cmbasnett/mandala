//mandala
#include "../app.hpp"
#include "../bitmap_font.hpp"
#include "../gui_label.hpp"
#include "../gui_image.hpp"

//armada
#include "console_state.hpp"

//boost
#include <boost\algorithm\string.hpp>

namespace mandala
{
	namespace armada
	{
		console_state_t::console_state_t()
		{
			root_node = std::make_shared<gui_node_t>();
			root_node->set_dock_mode(gui_dock_mode_e::top);
			root_node->set_size(gui_node_t::size_type(0, layout->bounds().size().y / 2));

			auto root_bg_image = std::make_shared<gui_image_t>();
			root_bg_image->set_dock_mode(gui_dock_mode_e::fill);
			root_bg_image->set_sprite(sprite_t(sprite_ref_t(hash_t("white.json"), hash_t("white.png"))));
			root_bg_image->set_color(vec4_t(0, 0, 0, 0.5f));

			input_label = std::make_shared<gui_label_t>();
			input_label->set_bitmap_font(app.resources.get<bitmap_font_t>(hash_t("terminal_16.fnt")));
			input_label->set_size(vec2_t(0, input_label->bitmap_font()->line_height));
			input_label->set_dock_mode(gui_dock_mode_e::bottom);
			input_label->set_padding(padding_t(16));
			input_label->set_string(L"test");

			output_label = std::make_shared<gui_label_t>();
			output_label->set_bitmap_font(app.resources.get<bitmap_font_t>(hash_t("terminal_16.fnt")));
			output_label->set_dock_mode(gui_dock_mode_e::fill);
			output_label->set_padding(padding_t(16));
			output_label->set_vertical_alignment(gui_label_t::vertical_alignment_e::bottom);
			output_label->set_justification(gui_label_t::justification_e::left);
			output_label->set_string(L"test");

			root_node->adopt(root_bg_image);
			root_node->adopt(input_label);
			root_node->adopt(output_label);

			layout->adopt(root_node);
		}

		void console_state_t::on_input_event(input_event_t& input_event)
		{
			gui_state_t::on_input_event(input_event);

			if (input_event.is_consumed)
			{
				return;
			}

			if (input_event.device_type == input_event_t::device_type_e::keyboard &&
				input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_press &&
				input_event.keyboard.key == input_event_t::keyboard_t::key_e::f1)
			{
				app.states.pop(shared_from_this());
				input_event.is_consumed = true;
			}
		}
	}
}
