#pragma once

//mandala
#include "gui_textfield.hpp"
#include "input_event.hpp"

namespace mandala
{
    void gui_textfield_t::on_input_event(input_event_t& input_event)
    {
        if (!has_focus)
        {
            return;
        }

        auto string_copy = string();

        if (input_event.device_type == input_event_t::device_type_e::keyboard)
        {
            if (input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_press ||
                input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_repeat)
            {
                switch (input_event.keyboard.key)
                {
                case input_event_t::keyboard_t::key_e::backspace:
                {
                    if (!string_copy.empty())
                    {
                        string_copy.resize(string_copy.length() - 1);
                    }

                    input_event.is_consumed = true;

                    break;
                }
                case input_event_t::keyboard_t::key_e::enter:
                case input_event_t::keyboard_t::key_e::kp_enter:
                {
                    string_copy += L"\n";

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
                default:
                    break;
                }
            }
            else if (input_event.keyboard.type == input_event_t::keyboard_t::type_e::character)
            {
                string_copy += input_event.keyboard.character;

                ++cursor.column_index;

                input_event.is_consumed = true;
            }

            if (input_event.is_consumed)
            {
                set_string(string_copy);
            }
        }
    }
}
