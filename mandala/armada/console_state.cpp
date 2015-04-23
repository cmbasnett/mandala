//mandala
#include "../resource_mgr.hpp"
#include "../state_mgr.hpp"
#include "../python_mgr.hpp"
#include "../bitmap_font.hpp"
#include "../gui_label.hpp"
#include "../gui_image.hpp"
#include "../color.hpp"

//armada
#include "console_state.hpp"

//boost
#include <boost\algorithm\string.hpp>
#include <boost\network.hpp>

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
            root_node->set_size(vec2_t(0.0, 0.75f), gui_size_mode_e::relative);

            auto output_root_node = std::make_shared<gui_node_t>();
            output_root_node->set_dock_mode(gui_dock_mode_e::fill);

            auto root_background_image = std::make_shared<gui_image_t>();
            root_background_image->set_color(rgba_type(rgb_type(0), 0.75f));
            root_background_image->set_dock_mode(gui_dock_mode_e::fill);
            root_background_image->set_sprite(sprite_t(sprite_ref_t(hash_t("white.tpsb"), hash_t("white.png"))));

            output_label = std::make_shared<gui_label_t>();
            output_label->set_bitmap_font(resources.get<bitmap_font_t>(hash_t("unifont_14.fnt")));
            output_label->set_dock_mode(gui_dock_mode_e::fill);
            output_label->set_justification(gui_label_t::justification_e::left);
            output_label->set_margin(padding_t(8));
            output_label->set_should_use_ellipses(false);
            output_label->set_should_use_color_codes(true);
            output_label->set_vertical_alignment(gui_label_t::vertical_alignment_e::bottom);

            output_root_node->adopt(output_label);

            auto input_root_node = std::make_shared<gui_node_t>();
            input_root_node->set_dock_mode(gui_dock_mode_e::bottom);
            input_root_node->set_size(vec2_t(0, resources.get<bitmap_font_t>(hash_t("unifont_14.fnt"))->get_line_height() + 16)); //HACK: we don't have parent resizing yet

            auto input_background_image = std::make_shared<gui_image_t>();
            input_background_image->set_color(rgba_type(0, 0, 0, 1));
            input_background_image->set_dock_mode(gui_dock_mode_e::fill);
            input_background_image->set_sprite(sprite_t(sprite_ref_t(hash_t("white.tpsb"), hash_t("white.png"))));

            input_label = std::make_shared<gui_label_t>();
			input_label->set_bitmap_font(resources.get<bitmap_font_t>(hash_t("unifont_14.fnt")));
			input_label->set_dock_mode(gui_dock_mode_e::fill);
			input_label->set_should_use_color_codes(false);
			input_label->set_should_use_ellipses(false);
			input_label->set_is_read_only(false);
            input_label->set_margin(padding_t(0, 8, 0, 8));
            input_label->set_vertical_alignment(gui_label_t::vertical_alignment_e::middle);
            input_label->set_color(rgba_type(1));

            input_root_node->adopt(input_background_image);
            input_root_node->adopt(input_label);

            root_node->adopt(root_background_image);
            root_node->adopt(input_root_node);
            root_node->adopt(output_root_node);

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
                    states.pop(shared_from_this());

                    input_event.is_consumed = true;

                    return;
                case input_event_t::keyboard_t::key_e::up:
                    if (command_strings_itr != command_strings.end())
                    {
						input_label->set_string(*command_strings_itr);

                        ++command_strings_itr;
                    }

                    //set cursor to end of line

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
					if ((input_event.keyboard.mod_flags & input_event_t::mod_flag_shift) == input_event_t::mod_flag_shift)
					{
						break;
					}

					if (!input_label->get_string().empty())
					{
						auto output_label_string = output_label->get_string();
						auto input_string_escaped = input_label->get_string_escaped();

						output_label_string.append(L"\n" + input_string_escaped);

						command_strings.push_front(input_label->get_string());

                        std::wstring_convert<std::codecvt_utf8<wchar_t>> wstring_convert;

						try
						{
							const auto command = wstring_convert.to_bytes(input_label->get_string());

							python.exec(command);
						}
						catch (const std::exception& exception)
						{
							output_label_string.append(L"\n↑" + rgb_to_hex<wchar_t>(error_color) + wstring_convert.from_bytes(std::string(exception.what())) + L"↓");
						}

						output_label->set_string(output_label_string);

						command_strings_itr = command_strings.begin();

						input_label->set_string(L"");
					}

                    input_event.is_consumed = true;

                    return;
                }
                case input_event_t::keyboard_t::key_e::escape:
                {
                    command_strings_itr = command_strings.begin();

					input_label->set_string(L"");

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