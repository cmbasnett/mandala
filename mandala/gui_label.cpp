#pragma once

//std
#include <sstream>
#include <codecvt>

//glm
#include <glm\glm.hpp>
#include <glm\ext.hpp>

//boost
#include <boost\algorithm\string.hpp>

//mandala
#include "bitmap_font.hpp"
#include "gui_label.hpp"
#include "color.hpp"
#include "collision.hpp"
#include "input_event.hpp"
#include "platform.hpp"
#include "gpu_program_mgr.hpp"
#include "basic_gpu_program.hpp"

namespace mandala
{
    std::wstring& sanitize_string(std::wstring& string)
    {
        auto string_itr = string.begin();

        while (string_itr != string.end())
        {
            if (*string_itr == color_push_character<wchar_t>::value)
            {
                //encountered color push character
                string_itr = string.erase(string_itr);

                if (string_itr != string.end() && *string_itr != color_push_character<wchar_t>::value)
                {
                    //determine how many characters to erase (up to 6)
                    auto erase_count = std::min(std::distance(string_itr, string.end()), 6);

                    //erase color code from string
                    string_itr = string.erase(string_itr, string_itr + erase_count);

                    continue;
                }
            }
            else if (*string_itr == color_pop_character<wchar_t>::value)
            {
                //encountered color pop character
                string_itr = string.erase(string_itr);

                continue;
            }

            if (string_itr != string.end())
            {
                ++string_itr;
            }
        }

        return string;
    }

    std::wstring& escape_string(std::wstring& string)
    {
        auto string_itr = string.begin();

        while (string_itr != string.end())
        {
            if (*string_itr == color_push_character<wchar_t>::value)
            {
                string_itr = string.insert(string_itr, color_push_character<wchar_t>::value) + 1;
            }
            else if (*string_itr == color_pop_character<wchar_t>::value)
            {
                string_itr = string.insert(string_itr, color_pop_character<wchar_t>::value) + 1;
            }

            if (string_itr != string.end())
            {
                ++string_itr;
            }
        }

        return string;
    }


    std::shared_ptr<vertex_buffer_t<basic_gpu_vertex_t>> _selection_vertex_buffer;
    std::shared_ptr<index_buffer_t<uint8_t>> _selection_index_buffer;

    gui_label_t::gui_label_t() :
        _cursor_vertex_buffer(std::make_shared<vertex_buffer_t<basic_gpu_vertex_t>>()),
        _cursor_index_buffer(std::make_shared<index_buffer_t<uint8_t>>()),
        _selection_vertex_buffer(std::make_shared<vertex_buffer_t<basic_gpu_vertex_t>>()),
        _selection_index_buffer(std::make_shared<index_buffer_t<uint8_t>>())
    {
        //cursor
        {
            //index buffer
            _cursor_index_buffer->data({ 0, 1 }, gpu_t::buffer_usage_e::static_draw);

            //vertex buffer
            std::initializer_list<basic_gpu_vertex_t> vertices =
            {
                basic_gpu_vertex_t(vec3_t(0, 0, 0), rgba_type(1)),
                basic_gpu_vertex_t(vec3_t(0, 1, 0), rgba_type(1)),
            };
            _cursor_vertex_buffer->data(vertices, gpu_t::buffer_usage_e::static_draw);
        }

        //selection
        {
            //index buffer
            _selection_index_buffer->data({ 0, 1, 2, 3 }, gpu_t::buffer_usage_e::static_draw);

            //vertex buffer
            std::initializer_list<basic_gpu_vertex_t> vertices =
            {
                basic_gpu_vertex_t(vec3_t(0, 0, 0), rgba_type(1)),
                basic_gpu_vertex_t(vec3_t(0, 1, 0), rgba_type(1)),
                basic_gpu_vertex_t(vec3_t(1, 1, 0), rgba_type(1)),
                basic_gpu_vertex_t(vec3_t(1, 0, 0), rgba_type(1)),
            };
            _selection_vertex_buffer->data(vertices, gpu_t::buffer_usage_e::static_draw);
        }

        _cursor.string_begin = _string.begin();
        _cursor.string_end = _string.end();
        _cursor.time_point = cursor_data_t::clock_type::now();
    }

    gui_label_t::string_type gui_label_t::get_string_escaped() const
    {
        string_type string = _string;

        escape_string(string);

        return string;
    }

    gui_label_t::string_type gui_label_t::get_string_sanitized() const
    {
        string_type string = _string;

        sanitize_string(string);

        return string;
    }

    gui_label_t::line_height_type gui_label_t::get_line_height() const
    {
        return _bitmap_font->get_line_height() + _line_spacing;
    }

    size_t gui_label_t::get_line_count() const
    {
        return _lines.size();
    }

    const gui_label_t::string_type& gui_label_t::get_line(size_t index) const
    {
        if (index < _lines.size() || index >= _lines.size())
        {
            throw std::out_of_range("");
        }

        return _lines[index].render_string;
    }

    void gui_label_t::clean()
    {
        static const auto fallback_character = L'?';
        static const auto ellipse_character = L'.';
        static const auto ellipses_max = 3;

        gui_node_t::clean();

        if (_bitmap_font == nullptr)
        {
            throw std::exception();
        }

        auto string_itr = _string.begin();
        const auto padded_size = (bounds() - padding()).size();
        const auto line_height = get_line_height();

        _lines.clear();

        while (string_itr != _string.end())
        {
            bool will_overflow = (_lines.size() + 1) * line_height > padded_size.y;

            if (will_overflow || (!_is_multiline && !_lines.empty()))
            {
                //adding another line would exceed maximum height or line count
                auto& line_string = _lines.back().render_string;

                if (_should_use_ellipses && !_lines.empty() && !line_string.empty())
                {
                    //attempt to add ellipses to the end of the last line
                    const auto ellipse_width = _bitmap_font->get_characters().at(ellipse_character).advance_x;
                    const auto ellipse_count = glm::min(static_cast<decltype(ellipse_width)>(padded_size.x) / ellipse_width, ellipses_max);
                    auto width = 0;

                    auto string_reverse_itr = line_string.rbegin() + 1;

                    //travel backwards from the end and calculate what part of the string will be replaced with ellipses
                    while (string_reverse_itr != line_string.rend())
                    {
                        width += _bitmap_font->get_characters().at(*string_reverse_itr).advance_x;

                        if (width >= (ellipse_count * ellipse_width))
                        {
                            break;
                        }

                        ++string_reverse_itr;
                    }

                    //TODO: this has an undesired effect on cursor, will need to be fixed
                    line_string.erase(string_reverse_itr.base() + 1, line_string.end());
                    line_string.append(ellipse_count, ellipse_character);
                }

                break;
            }

            line_t::width_type line_width = 0;
            auto string_begin = string_itr;
            auto string_end = string_itr;
            auto was_line_added = false;
            auto string_space_itr = _string.end();

            auto parse_color_codes = [&](string_type& string, std::vector<std::pair<size_t, vec4_t>>& color_pushes, std::vector<size_t>& color_pops)
            {
                auto string_itr = string.begin();

                while (string_itr != string.end())
                {
                    if (*string_itr == color_push_character<string_type::value_type>::value)
                    {
                        //encountered color push character
                        string_itr = string.erase(string_itr);

                        if (string_itr != string.end() && *string_itr != color_push_character<string_type::value_type>::value)
                        {
                            //determine how many characters to erase
                            auto erase_count = std::min(std::distance(string_itr, string.end()), rgb_hex_string_length);

                            if (erase_count == rgb_hex_string_length)
                            {
                                //try to parse a hex color from string
                                string_type hex_string = { string_itr, string_itr + rgb_hex_string_length };

                                try
                                {
                                    auto color = rgba_type(hex_to_rgb(hex_string), 1);

                                    color_pushes.push_back(std::make_pair(std::distance(string.begin(), string_itr), color));
                                }
                                catch (...)
                                {
                                    //conversion failed
                                }
                            }

                            //erase color code from string
                            string_itr = string.erase(string_itr, string_itr + erase_count);

                            continue;
                        }
                    }
                    else if (*string_itr == color_pop_character<string_type::value_type>::value)
                    {
                        //encountered color pop character
                        string_itr = string.erase(string_itr);

                        if (string_itr == string.end() || *string_itr != color_pop_character<string_type::value_type>::value)
                        {
                            color_pops.push_back(std::distance(string.begin(), string_itr));

                            continue;
                        }
                    }

                    if (string_itr != string.end())
                    {
                        ++string_itr;
                    }
                }
            };

            auto replace_unrecognized_characters = [&](string_type& string)
            {
                auto string_itr = string.begin();

                while (string_itr != string.end())
                {
                    if (_bitmap_font->get_characters().find(*string_itr) == _bitmap_font->get_characters().end())
                    {
                        string.replace(string_itr, string_itr + 1, 1, fallback_character);
                    }

                    ++string_itr;
                }
            };

            auto add_line = [&](string_itr_type string_begin, string_itr_type string_end)
            {
                line_t line;
                line.string_begin = string_begin;
                line.string_end = string_end;

                string_type render_string = { string_begin, string_end };

                //strip color codes from string
                if (_should_use_color_codes)
                {
                    parse_color_codes(render_string, line.colors_pushes, line.color_pop_indices);
                }

                replace_unrecognized_characters(render_string);

                auto render_string_width = _bitmap_font->get_string_width(render_string);

                line.render_string = render_string;
                line.rectangle.width = render_string_width;
                line.rectangle.height = _bitmap_font->get_line_height();
                line.rectangle.x = 0;
                line.rectangle.y = static_cast<float32_t>(_lines.size()) * -get_line_height();

                switch (_justification)
                {
                case justification_e::center:
                    line.rectangle.x -= render_string_width / 2;
                    break;
                case justification_e::right:
                    line.rectangle.x -= render_string_width;
                    break;
                default:
                    break;
                }

                _lines.emplace_back(line);

                was_line_added = true;
            };

            while (string_itr != _string.end())
            {
                if (*string_itr == L'\n')
                {
                    add_line(string_begin, string_itr);

                    ++string_itr;

                    if (string_itr == _string.end())
                    {
                        //add empty line
                        add_line(string_itr, string_itr);
                    }

                    break;
                }
                else if (*string_itr == L' ' || *string_itr == L'-')
                {
                    string_space_itr = string_itr;
                }

                int16_t character_width = 0;

                if (_should_use_color_codes)
                {
                    if (*string_itr == color_push_character<string_type::value_type>::value)
                    {
                        //encountered color push character

                        //skip to next character
                        ++string_itr;

                        if (string_itr == _string.end())
                        {
                            break;
                        }
                        else if (*string_itr != color_push_character<string_type::value_type>::value)
                        {
                            string_itr += std::min(std::distance(string_itr, _string.end()), rgb_hex_string_length);

                            continue;
                        }
                    }
                    else if (*string_itr == color_pop_character<string_type::value_type>::value)
                    {
                        //encountered color pop character
                        ++string_itr;

                        if (string_itr == _string.end())
                        {
                            break;
                        }
                        else if (*string_itr != color_pop_character<string_type::value_type>::value)
                        {
                            continue;
                        }
                    }
                }

                uint16_t character_id = *string_itr;

                auto characters_itr = _bitmap_font->get_characters().find(character_id);

                string_type::value_type character;

                if (characters_itr == _bitmap_font->get_characters().end())
                {
                    character = fallback_character;
                }
                else
                {
                    character = *string_itr;
                }

                character_width = _bitmap_font->get_characters().at(character).advance_x;

                if (line_width + character_width > padded_size.x)
                {
                    if (string_space_itr != _string.end())
                    {
                        add_line(string_begin, string_space_itr);

                        string_itr = string_space_itr + 1;
                    }

                    break;
                }

                line_width += character_width;

                ++string_itr;
            }

            if (!was_line_added)
            {
                add_line(string_begin, string_itr);
            }
        }

        if (_lines.empty())
        {
            //HACK: if no lines, add one in so that the cursor can draw on empty lines
            //TODO: consolidate this with the normal add_line function
            line_t line;
            line.string_begin = _string.begin();
            line.string_end = _string.end();
            line.rectangle.height = _bitmap_font->get_line_height();

            _lines.emplace_back(line);
        }

        //calculate base translation
        auto base_translation = bounds().min;
        base_translation.x += padding().left;
        base_translation.y += padding().bottom;

        switch (_vertical_alignment)
        {
        case vertical_alignment_e::top:
            base_translation.y += padded_size.y - _bitmap_font->get_base();
            break;
        case vertical_alignment_e::middle:
            base_translation.y += (padded_size.y / 2) - (_bitmap_font->get_base() / 2) + ((line_height * (_lines.size() - 1)) / 2);
            break;
        case vertical_alignment_e::bottom:
            base_translation.y += (line_height * _lines.size()) - _bitmap_font->get_base();
            break;
        default:
            break;
        }

        switch (_justification)
        {
        case justification_e::center:
            base_translation.x += padded_size.x / 2;
            break;
        case justification_e::right:
            base_translation.x += padded_size.x;
            break;
        default:
            break;
        }

        //add base translation to all line rectangles
        for (auto& line : _lines)
        {
            line.rectangle.x += base_translation.x;
            line.rectangle.y += base_translation.y;
        }

        update_cursor();
    }

    void gui_label_t::render_override(mat4_t world_matrix, mat4_t view_projection_matrix)
    {
        if (_bitmap_font == nullptr)
        {
            throw std::exception("bitmap font not set");
        }

        std::stack<rgba_type> color_stack;

        for (const auto& line : _lines)
        {
            const auto line_world_matrix = world_matrix * glm::translate(line.rectangle.x, line.rectangle.y, 0.0f);

            _bitmap_font->render_string(line.render_string, line_world_matrix, view_projection_matrix, color(), color_stack, line.colors_pushes, line.color_pop_indices);
        }

        //render cursor
        if (!is_read_only())
        {
            bool should_show_cursor = ((_cursor.time_point - cursor_data_t::clock_type::now()).count() / (cursor_data_t::clock_type::period::den / 2)) % 2 == 0;

            if (should_show_cursor)
            {
                gpu.buffers.push(gpu_t::buffer_target_e::array, _cursor_vertex_buffer);
                gpu.buffers.push(gpu_t::buffer_target_e::element_array, _cursor_index_buffer);

                auto cursor_world_matrix = world_matrix;

                cursor_world_matrix *= glm::translate(_cursor.rectangle.x, _cursor.rectangle.y + static_cast<float32_t>(_bitmap_font->get_base() - _bitmap_font->get_line_height()), 0.0f);
                cursor_world_matrix *= glm::scale(1.0f, static_cast<float32_t>(get_line_height()), 1.0f);

                const auto& gpu_program = gpu_programs.get<basic_gpu_program_t>();

                gpu.programs.push(gpu_program);

                gpu_program->world_matrix(cursor_world_matrix);
                gpu_program->view_projection_matrix(view_projection_matrix);

                gpu.draw_elements(gpu_t::primitive_type_e::lines, 2, index_buffer_type::data_type, 0);

                gpu.programs.pop();

                gpu.buffers.pop(gpu_t::buffer_target_e::element_array);
                gpu.buffers.pop(gpu_t::buffer_target_e::array);
            }
        }

        gui_node_t::render_override(world_matrix, view_projection_matrix);
    }

    void gui_label_t::on_input_event(input_event_t& input_event)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wstring_convert;

        if (input_event.device_type == input_event_t::device_type_e::touch &&
            input_event.touch.type == input_event_t::touch_t::type_e::press)
        {
            if (contains(bounds(), input_event.touch.position))
            {
                //std::cout << "touched a label" << std::endl;
            }
        }

        if (!is_read_only() /*&& has_focus()*/)
        {
            if (input_event.device_type == input_event_t::device_type_e::keyboard)
            {
                if (input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_press ||
                    input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_repeat)
                {
                    switch (input_event.keyboard.key)
                    {
                    case input_event_t::keyboard_t::key_e::backspace:
                    {
                        if (_string.empty())
                        {
                            break;
                        }

                        if (_cursor.string_begin != _cursor.string_end)
                        {
                            _cursor.string_begin = _string.erase(_cursor.string_begin, _cursor.string_end);
                            _cursor.string_end = _cursor.string_begin;

                            dirty();
                        }
                        else if (_cursor.string_begin > _string.begin())
                        {
                            _cursor.string_begin = _string.erase(_cursor.string_begin - 1);
                            _cursor.string_end = _cursor.string_begin;

                            dirty();
                        }

                        input_event.is_consumed = true;

                        break;
                    }
                    case input_event_t::keyboard_t::key_e::enter:
                    case input_event_t::keyboard_t::key_e::kp_enter:
                    {
                        _cursor.string_begin = _string.insert(_cursor.string_begin, L'\n') + 1;
                        _cursor.string_end = _cursor.string_begin;

                        dirty();

                        input_event.is_consumed = true;

                        break;
                    }
                    case input_event_t::keyboard_t::key_e::home:
                    {
                        //TODO: this is a bit inefficient as it requires iteration over all lines
                        //would be more expedient if we dealt with finding the line the cursor is
                        //on during some sort of cleaning phase?
                        for (auto& line : _lines)
                        {
                            if (_cursor.string_begin >= line.string_begin && _cursor.string_begin <= line.string_end)
                            {
                                if (input_event.keyboard.mod_flags == input_event_t::mod_flag_shift)
                                {
                                    _cursor.string_end = _cursor.string_begin;
                                    _cursor.string_begin = line.string_begin;
                                }
                                else
                                {
                                    _cursor.string_begin = line.string_begin;
                                    _cursor.string_end = _cursor.string_begin;
                                }

                                update_cursor();

                                break;
                            }
                        }

                        break;
                    }
                    case input_event_t::keyboard_t::key_e::end:
                    {
                        //TODO: this is a bit inefficient as it requires iteration over all lines
                        //would be more expedient if we dealt with finding the line the cursor is
                        //on during some sort of cleaning phase?
                        for (auto& line : _lines)
                        {
                            if (_cursor.string_begin >= line.string_begin && _cursor.string_begin <= line.string_end)
                            {
                                if (input_event.keyboard.mod_flags == input_event_t::mod_flag_shift)
                                {
                                    _cursor.string_end = line.string_end;
                                }
                                else
                                {
                                    _cursor.string_begin = line.string_end;
                                    _cursor.string_end = _cursor.string_begin;
                                }

                                update_cursor();

                                break;
                            }
                        }

                        break;
                    }
                    case input_event_t::keyboard_t::key_e::del:
                    {
                        if (_cursor.string_begin != _cursor.string_end)
                        {
                            _cursor.string_begin = _string.erase(_cursor.string_begin, _cursor.string_end);
                        }
                        else
                        {
                            _cursor.string_begin = _string.erase(_cursor.string_begin);
                        }

                        _cursor.string_end = _cursor.string_begin;

                        dirty();

                        input_event.is_consumed = true;

                        break;
                    }
                    case input_event_t::keyboard_t::key_e::left:
                    {
                        if (_cursor.string_begin != _string.begin())
                        {
                            --_cursor.string_begin;

                            if (input_event.keyboard.mod_flags != input_event_t::mod_flag_shift)
                            {
                                _cursor.string_end = _cursor.string_begin;
                            }

                            update_cursor();
                        }

                        break;
                    }
                    case input_event_t::keyboard_t::key_e::right:
                    {
                        if (_cursor.string_begin < _string.end())
                        {
                            _cursor.string_end = _cursor.string_begin + 1;

                            if (input_event.keyboard.mod_flags != input_event_t::mod_flag_shift)
                            {
                                _cursor.string_begin = _cursor.string_end;
                            }

                            update_cursor();
                        }

                        break;
                    }
                    case input_event_t::keyboard_t::key_e::down:
                    {
                        //TODO: set cursor to the same column on the next line

                        break;
                    }
                    case input_event_t::keyboard_t::key_e::up:
                    {
                        //TODO: set cursor to the same column on the previous line

                        break;
                    }
                    case input_event_t::keyboard_t::key_e::v:
                        if (input_event.keyboard.mod_flags == input_event_t::mod_flag_ctrl)
                        {
                            const auto clipboard_string = wstring_convert.from_bytes(platform.get_clipboard_string().c_str());

                            _cursor.string_begin = _string.insert(_cursor.string_begin, clipboard_string.begin(), clipboard_string.end()) + clipboard_string.size();
                            _cursor.string_end = _cursor.string_begin;

                            dirty();

                            input_event.is_consumed = true;
                        }
                        break;
                    case input_event_t::keyboard_t::key_e::c:
                        if (input_event.keyboard.mod_flags == input_event_t::mod_flag_ctrl)
                        {
                            string_type string = { _cursor.string_begin, _cursor.string_end };
                            platform.set_clipboard_string(wstring_convert.to_bytes(string.c_str()));

                            input_event.is_consumed = true;
                        }
                        break;
                    case input_event_t::keyboard_t::key_e::x:
                    {
                        if (input_event.keyboard.mod_flags == input_event_t::mod_flag_ctrl)
                        {
                            string_type string = { _cursor.string_begin, _cursor.string_end };
                            platform.set_clipboard_string(wstring_convert.to_bytes(string.c_str()));

                            _cursor.string_begin = _string.erase(_cursor.string_begin, _cursor.string_end);
                            _cursor.string_end = _cursor.string_begin;

                            dirty();

                            input_event.is_consumed = true;
                        }
                        break;
                    }
                    case input_event_t::keyboard_t::key_e::a:
                    {
                        if (input_event.keyboard.mod_flags == input_event_t::mod_flag_ctrl)
                        {
                            _cursor.string_begin = _string.begin();
                            _cursor.string_end = _string.end();

                            update_cursor();

                            input_event.is_consumed = true;
                        }
                        break;
                    }
                    default:
                        break;
                    }
                }
                else if (input_event.keyboard.type == input_event_t::keyboard_t::type_e::character)
                {
                    if (_cursor.string_begin != _cursor.string_end)
                    {
                        _cursor.string_begin = _string.erase(_cursor.string_begin, _cursor.string_end);
                    }

                    _cursor.string_begin = _string.insert(_cursor.string_begin, input_event.keyboard.character);
                    _cursor.string_end = ++_cursor.string_begin;

                    dirty();
                    
                    input_event.is_consumed = true;
                }
            }
        }
    }

    void gui_label_t::update_cursor()
    {
        for (const auto& line : _lines)
        {
            bool a = (_cursor.string_begin >= line.string_begin);
            bool b = (_cursor.string_begin <= line.string_end);
            bool is_cursor_on_line = (a && b);

            if (is_cursor_on_line)
            {
                _cursor.rectangle.x = line.rectangle.x + _bitmap_font->get_string_width({ line.string_begin, _cursor.string_begin });
                _cursor.rectangle.y = line.rectangle.y;
                _cursor.rectangle.height = _bitmap_font->get_line_height();
                _cursor.rectangle.width = 1;
            }
        }

        _cursor.time_point = cursor_data_t::clock_type::now();
    }
}