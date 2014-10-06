﻿//mandala
#include "../app.hpp"
#include "../bitmap_font.hpp"
#include "../gui_label.hpp"
#include "../gui_image.hpp"
#include "../gui_textfield.hpp"
#include "../color.hpp"

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
        vec3_t console_state_t::echo_color = color_white;
        vec3_t console_state_t::warning_color = color_yellow;
        vec3_t console_state_t::error_color = color_red;

		console_state_t::console_state_t()
        {
			root_node = std::make_shared<gui_node_t>();
			root_node->set_dock_mode(gui_dock_mode_e::top);
			root_node->set_size(gui_node_t::size_type(0, layout->bounds().size().y / 2));   //HACK: we don't have proportial sizes yet

            auto output_root_node = std::make_shared<gui_node_t>();
            output_root_node->set_dock_mode(gui_dock_mode_e::fill);

            auto root_background_image = std::make_shared<gui_image_t>();
            root_background_image->set_color(vec4_t(vec3_t(0), 0.75f));
            root_background_image->set_dock_mode(gui_dock_mode_e::fill);
            root_background_image->set_sprite(sprite_t(sprite_ref_t(hash_t("white.json"), hash_t("white.png"))));

            output_label = std::make_shared<gui_label_t>();
            output_label->set_bitmap_font(app.resources.get<bitmap_font_t>(hash_t("inconsolata_12.fnt")));
            output_label->set_dock_mode(gui_dock_mode_e::fill);
            output_label->set_vertical_alignment(gui_label_t::vertical_alignment_e::bottom);
            output_label->set_justification(gui_label_t::justification_e::left);
            //output_label->set_line_spacing(4);
            output_label->set_margin(padding_t(8, 8, -32, 8));
            output_label->set_should_use_ellipses(false);
            output_label->set_should_use_color_codes(true);

            output_root_node->adopt(output_label);

            auto input_root_node = std::make_shared<gui_node_t>();
            input_root_node->set_dock_mode(gui_dock_mode_e::bottom);
            input_root_node->set_size(vec2_t(0, app.resources.get<bitmap_font_t>(hash_t("inconsolata_12.fnt"))->line_height + 16)); //HACK: we don't have parent resizing yet

            auto input_background_image = std::make_shared<gui_image_t>();
            input_background_image->set_sprite(sprite_t(sprite_ref_t(hash_t("white.json"), hash_t("white.png"))));
            input_background_image->set_dock_mode(gui_dock_mode_e::fill);
            input_background_image->set_color(vec4_t(vec3_t(0), 0.5f));

            input_textfield = std::make_shared<gui_textfield_t>();
            input_textfield->set_bitmap_font(app.resources.get<bitmap_font_t>(hash_t("inconsolata_12.fnt")));
            input_textfield->set_dock_mode(gui_dock_mode_e::fill);
            input_textfield->set_should_use_color_codes(false);
            input_textfield->set_should_use_ellipses(false);

            input_root_node->adopt(input_textfield);

            input_root_node->adopt(input_background_image);

            //auto root_border_image = std::make_shared<gui_image_t>();
            //root_border_image->set_color(vec4_t(vec3_t(0), 1));
            //root_border_image->set_dock_mode(gui_dock_mode_e::bottom);
            //root_border_image->set_sprite(sprite_t(sprite_ref_t(hash_t("white.json"), hash_t("white.png"))));
            //root_border_image->set_is_autosized_to_texture(false);
            //root_border_image->set_size(vec2_t(0, 2));

            root_node->adopt(root_background_image);
            //root_node->adopt(root_border_image);
            root_node->adopt(input_root_node);
            root_node->adopt(output_root_node);

			layout->adopt(root_node);

            layout->clean();
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
                    if (command_strings_itr != command_strings.end())
                    {
                        input_textfield->set_string(*command_strings_itr);

                        ++command_strings_itr;
                    }

                    input_event.is_consumed = true;
                    return;
                case input_event_t::keyboard_t::key_e::down:
                    {
                        auto command_strings_reverse_itr = std::deque<command_string_type>::reverse_iterator(command_strings_itr);
                    }

                    input_event.is_consumed = true;
                    return;
                case input_event_t::keyboard_t::key_e::enter:
                case input_event_t::keyboard_t::key_e::kp_enter:
                {
                    if (!input_textfield->string().empty())
                    {
                        auto output_label_string = output_label->string();
                        output_label_string.append(L"\n" + input_textfield->string());

                        command_strings.push_front(input_textfield->string());

                        typedef std::codecvt_utf8<wchar_t> convert_type;
                        std::wstring_convert<convert_type, wchar_t> converter;

                        try
                        {
                            app.lua.execute(converter.to_bytes(input_textfield->string()));
                        }
                        catch (const std::exception& exception)
                        {
                            output_label_string.append(L"\n↑" +
                                rgb_to_hex<wchar_t>(error_color) + converter.from_bytes(std::string(exception.what())) + L"↓");
                        }

                        output_label->set_string(output_label_string);

                        command_strings_itr = command_strings.begin();

                        input_textfield->set_string(L"");
                    }

                    input_event.is_consumed = true;
                    return;
                }
                case input_event_t::keyboard_t::key_e::escape:
                {
                    command_strings_itr = command_strings.begin();

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
