#pragma once

//mandala
#include "gui_textfield.hpp"
#include "input_event.hpp"
#include "platform.hpp"

//std
#include <codecvt>

namespace mandala
{
    void gui_textfield_t::on_input_event(input_event_t& input_event)
    {
        if (!has_focus)
        {
            return;
        }

        if (input_event.device_type == input_event_t::device_type_e::keyboard)
        {
            if (input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_press ||
                input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_repeat)
            {
                switch (input_event.keyboard.key)
                {
                case input_event_t::keyboard_t::key_e::backspace:
                {
                    if (!string().empty())
                    {
                        auto string_copy = string();
                        string_copy.resize(string_copy.length() - 1);
                        set_string(string_copy);
                    }

                    input_event.is_consumed = true;

                    break;
                }
                case input_event_t::keyboard_t::key_e::enter:
                case input_event_t::keyboard_t::key_e::kp_enter:
                {
                    auto string_copy = string();
                    string_copy += L"\n";
                    set_string(string_copy);

                    input_event.is_consumed = true;

                    break;
                }
                case input_event_t::keyboard_t::key_e::home:
                {
                    if ((input_event.keyboard.mod_flags & input_event_t::mod_flag_shift) == input_event_t::mod_flag_shift)
                    {
                    }

                    cursor.column_index = 0;

                    break;
                }
                case input_event_t::keyboard_t::key_e::end:
                {
                    cursor.column_index = 0;    //TODO: make this do whatever

                    break;
                }
                case input_event_t::keyboard_t::key_e::v:
                    if (input_event.keyboard.mod_flags == input_event_t::mod_flag_ctrl)
                    {
                        const auto clipboard_string = platform.get_clipboard_string();

                        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

                        auto string_copy = string();
                        string_copy += converter.from_bytes(clipboard_string.c_str());
                        set_string(string_copy);

                        input_event.is_consumed = true;
                    }
                    break;
                case input_event_t::keyboard_t::key_e::c:
                    if (input_event.keyboard.mod_flags == input_event_t::mod_flag_ctrl)
                    {
                        //TODO: only copy selected text
                        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

                        platform.set_clipboard_string(converter.to_bytes(string()).c_str());

                        input_event.is_consumed = true;
                    }
                    break;
                default:
                    break;
                }
            }
            else if (input_event.keyboard.type == input_event_t::keyboard_t::type_e::character)
            {
                auto string_copy = string();
                string_copy += input_event.keyboard.character;
                set_string(string_copy);

                ++cursor.column_index;

                input_event.is_consumed = true;
            }
        }
    }
}
