//mandala
#include "../app.hpp"
#include "../bitmap_font.hpp"
#include "../gui_label.hpp"
#include "../gui_image.hpp"
#include "../gui_textfield.hpp"

//armada
#include "console_state.hpp"

//boost
#include <boost\algorithm\string.hpp>

//std
#include <codecvt>

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

            input_textfield = std::make_shared<gui_textfield_t>();
            input_textfield->set_bitmap_font(app.resources.get<bitmap_font_t>(hash_t("terminal_8.fnt")));
            input_textfield->set_size(vec2_t(0, input_textfield->bitmap_font()->line_height));
            input_textfield->set_dock_mode(gui_dock_mode_e::bottom);
			//input_label->set_margin(padding_t(16));

            output_label = std::make_shared<gui_label_t>();
            output_label->set_bitmap_font(app.resources.get<bitmap_font_t>(hash_t("terminal_8.fnt")));
            output_label->set_dock_mode(gui_dock_mode_e::fill);
            output_label->set_padding(padding_t(16));
            output_label->set_vertical_alignment(gui_label_t::vertical_alignment_e::bottom);
            output_label->set_justification(gui_label_t::justification_e::left);

            root_node->adopt(root_bg_image);
            root_node->adopt(input_textfield);
            root_node->adopt(output_label);

			layout->adopt(root_node);
		}

		void console_state_t::on_input_event(input_event_t& input_event)
        {
            if (input_event.device_type == input_event_t::device_type_e::keyboard &&
                input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_press)
            {
                switch (input_event.keyboard.key)
                {
                case input_event_t::keyboard_t::key_e::grave_accent:
                    app.states.pop(shared_from_this());

                    input_event.is_consumed = true;
                    return;
                case input_event_t::keyboard_t::key_e::up:
                    if (commands_itr != commands.end())
                    {
                        input_textfield->set_string(*commands_itr);

                        ++commands_itr;
                    }

                    input_event.is_consumed = true;
                    return;
                case input_event_t::keyboard_t::key_e::down:
                    //TODO: command history navigation
                    input_event.is_consumed = true;
                    return;
                case input_event_t::keyboard_t::key_e::enter:
                case input_event_t::keyboard_t::key_e::kp_enter:
                {
                    auto output_label_string = output_label->string();

                    output_label_string.append(L"\n" + input_textfield->string());

                    output_label->set_string(output_label_string);

                    commands.push_front(input_textfield->string());

                    typedef std::codecvt_utf8<wchar_t> convert_type;
                    std::wstring_convert<convert_type, wchar_t> converter;
                    auto command = converter.to_bytes(input_textfield->string());

                    app.lua.execute(command);

                    commands_itr = commands.begin();

                    input_textfield->set_string(L"");

                    input_event.is_consumed = true;
                    return;
                }
                case input_event_t::keyboard_t::key_e::escape:
                {
                    commands_itr = commands.begin();

                    input_textfield->set_string(L"");

                    input_event.is_consumed = true;
                    return;
                }
                default:
                    break;
                }
            }

			gui_state_t::on_input_event(input_event);
		}
	}
}
