﻿//std
#include <sstream>
#include <codecvt>

//glm
#include <glm\glm.hpp>
#include <glm\ext.hpp>

//boost
#include <boost\algorithm\string.hpp>

//naga
#include "bitmap_font.hpp"
#include "gui_label.hpp"
#include "color.hpp"
#include "collision.hpp"
#include "input_event.hpp"
#include "platform.hpp"
#include "gpu_program_mgr.hpp"
#include "basic_gpu_program.hpp"
#include "debug_renderer.hpp"

namespace naga
{
    GUILabel::StringType& GUILabel::sanitize_string(StringType& string)
    {
        auto string_itr = string.begin();

        while (string_itr != string.end())
        {
            if (*string_itr == color_push_character<GUILabel::StringType::value_type>::VALUE)
            {
                //encountered color push character
                string_itr = string.erase(string_itr);

                if (string_itr != string.end() && *string_itr != color_push_character<GUILabel::StringType::value_type>::VALUE)
                {
                    //determine how many characters to erase
                    auto erase_count = std::min(std::distance(string_itr, string.end()), rgb_hex_string_length);

                    //erase color code from string
                    string_itr = string.erase(string_itr, string_itr + erase_count);

                    continue;
                }
            }
            else if (*string_itr == color_pop_character<GUILabel::StringType::value_type>::VALUE)
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

    GUILabel::StringType& GUILabel::escape_string(StringType& string)
    {
        auto string_itr = string.begin();

        while (string_itr != string.end())
        {
            if (*string_itr == color_push_character<StringType::value_type>::VALUE)
            {
                string_itr = string.insert(string_itr, color_push_character<StringType::value_type>::VALUE) + 1;
            }
            else if (*string_itr == color_pop_character<StringType::value_type>::VALUE)
            {
                string_itr = string.insert(string_itr, color_pop_character<StringType::value_type>::VALUE) + 1;
            }

            if (string_itr != string.end())
            {
                ++string_itr;
            }
        }

        return string;
    }

	GUILabel::GUILabel()
    {
        cursor.string_begin = string.begin();
        cursor.string_end = string.end();
        cursor.time_point = cursor_data_t::clock_type::now();
    }

    GUILabel::LineHeightType GUILabel::get_line_height() const
    {
        return bitmap_font->get_line_height() + line_spacing;
    }

    size_t GUILabel::get_line_count() const
    {
        return lines.size();
    }

    void GUILabel::set_string(const StringType& string)
    {
        if (max_length)
        {
            this->string = { string.begin(), string.begin() + glm::min(max_length.get(), string.length()) };
        }
        else
        {
            this->string = string;
        }
        
        cursor.string_begin = this->string.begin() + this->string.length();
        cursor.string_end = this->string.begin() + this->string.length();
        
        dirty();
    }

    void GUILabel::set_max_length(const boost::optional<size_t>& max_length)
    {
        this->max_length = max_length;

        if (max_length && string.length() > max_length.get())
        {
            set_string(get_string());
        }
    }

    void GUILabel::on_clean_begin()
    {
        if (is_autosized_to_text)
        {
            update_lines();

            SizeType text_size;

            text_size.y = static_cast<f32>(lines.size() * get_line_height());

            for (auto& line : lines)
            {
                text_size.x = std::max(text_size.x, line.rectangle.width);
            }

			// TODO: setting the size mode is probably not what we want to happen, we want an
			// "internal" size mode, that is hidden from the user. the user would probably
			// not like it if the size mode was being overridden as a result of
			// is_autosized_to_text being true.
            set_size_modes(GUISizeModes(GUISizeMode::ABSOLUTE, GUISizeMode::ABSOLUTE));
            set_size(text_size);
        }
    }

    void GUILabel::on_clean_end()
    {
        if (!is_autosized_to_text)
        {
            update_lines();
        }

        const auto padded_size = (get_bounds() - get_padding()).size();
        const auto line_height = get_line_height();

        //calculate base translation
        auto base_translation = get_bounds().min;
        base_translation.x += get_padding().left;
        base_translation.y += get_padding().bottom;

        switch (vertical_alignment)
        {
        case VerticalAlignment::TOP:
            base_translation.y += padded_size.y - bitmap_font->get_base();
            break;
        case VerticalAlignment::MIDDLE:
            base_translation.y += (padded_size.y / 2) - (bitmap_font->get_base() / 2) + ((line_height * (lines.size() - 1)) / 2);
            break;
        case VerticalAlignment::BOTTOM:
            base_translation.y += (line_height * lines.size()) - bitmap_font->get_base();
            break;
        default:
            break;
        }

        switch (justification)
        {
        case Justification::CENTER:
            base_translation.x += padded_size.x / 2;
            break;
        case Justification::RIGHT:
            base_translation.x += padded_size.x;
            break;
        default:
            break;
        }

        base_translation = glm::round(base_translation);

		// TODO: this is 
        // add base translation to all line rectangles
        for (auto& line : lines)
        {
            line.rectangle.x += base_translation.x;
            line.rectangle.y += base_translation.y;
        }

        update_cursor();
    }

    void GUILabel::on_render_begin(mat4& world_matrix, mat4& view_projection_matrix)
    {
        if (bitmap_font == nullptr)
        {
            throw std::exception("bitmap font not set");
        }

        std::stack<vec4> color_stack;

        //round translation component of world_matrix (otherwise text can appear blurry)
        //TODO: figure out a nicer way to write this
        auto rounded_world_matrix = world_matrix;
        rounded_world_matrix[3][0] = glm::round(rounded_world_matrix[3][0]);
        rounded_world_matrix[3][1] = glm::round(rounded_world_matrix[3][1]);
        rounded_world_matrix[3][2] = glm::round(rounded_world_matrix[3][2]);

        for (const auto& line : lines)
        {
			const auto line_world_matrix = rounded_world_matrix * glm::translate(vec3(line.rectangle.x, line.rectangle.y, 0.0f));

            bitmap_font->render_string(line.render_string, line_world_matrix, view_projection_matrix, get_color(), color_stack, line.colors_pushes, line.color_pop_indices);
        }

		//TODO: render selection boxes for each line

        //render cursor
        if (!is_read_only)
        {
            bool should_show_cursor = ((cursor.time_point - cursor_data_t::clock_type::now()).count() / (cursor_data_t::clock_type::period::den / 2)) % 2 == 0;

            if (should_show_cursor)
            {
                render_rectangle(world_matrix, view_projection_matrix, cursor.rectangle, vec4(1, 1, 1, 1), true);
            }
        }

		GUINode::on_render_begin(world_matrix, view_projection_matrix);
    }

	bool GUILabel::on_input_event_begin(InputEvent& input_event)
    {
		if (input_event.type.device == InputDeviceType::MOUSE &&
			input_event.type.action == InputActionType::PRESS)
        {
            if (contains(get_bounds(), input_event.mouse.location()))
            {
                for (auto& line : lines)
                {
                    if (input_event.mouse.y >= line.rectangle.y &&
                        input_event.mouse.y <= (line.rectangle.y + line.rectangle.height))
                    {
                        if (input_event.mouse.x <= line.rectangle.x)
                        {
                            cursor.string_begin = line.string_begin;
                            cursor.string_end = cursor.string_begin;

                            update_cursor();

                            return true;
                        }
                        else if (input_event.mouse.x >= (line.rectangle.x + line.rectangle.width))
                        {
                            cursor.string_begin = line.string_end;
                            cursor.string_end = cursor.string_begin;

                            update_cursor();

                            return true;
                        }
                        else
                        {
                            auto x = line.rectangle.x;

                            for (auto string_itr = line.string_begin; string_itr != line.string_end; ++string_itr)
                            {
                                auto characters_itr = bitmap_font->get_characters().find(*string_itr);

                                if (characters_itr == bitmap_font->get_characters().end())
                                {
                                    characters_itr = bitmap_font->get_characters().find(L'?'); //TODO: get fallback character from somewhere else
                                }

                                if (input_event.mouse.x < x + (characters_itr->second.advance_x / 2))
                                {
                                    cursor.string_begin = string_itr;
                                    cursor.string_end = cursor.string_begin;

                                    update_cursor();

                                    break;
                                }
                                else if (input_event.mouse.x < x + characters_itr->second.advance_x)
                                {
                                    cursor.string_begin = string_itr + 1;
                                    cursor.string_end = cursor.string_begin;

                                    update_cursor();

                                    break;
                                }

                                x += characters_itr->second.advance_x;
                            }
                        }

                        break;
                    }
                }
            }
        }

        if (!is_read_only /*&& has_focus()*/)
        {
			if (input_event.type.device == InputDeviceType::KEYBOARD)
            {
				if (input_event.type.action == InputActionType::PRESS ||
					input_event.type.action == InputActionType::REPEAT)
                {
                    std::wstring_convert<std::codecvt_utf8<wchar_t>> wstring_convert;

                    switch (input_event.type.key)
                    {
					case Key::BACKSPACE:
                    {
                        if (string.empty())
                        {
                            break;
                        }

                        if (cursor.string_begin != cursor.string_end)
                        {
                            cursor.string_begin = string.erase(cursor.string_begin, cursor.string_end);
                            cursor.string_end = cursor.string_begin;

                            dirty();
                        }
                        else if (cursor.string_begin > string.begin())
                        {
                            cursor.string_begin = string.erase(cursor.string_begin - 1);
                            cursor.string_end = cursor.string_begin;

                            dirty();
                        }

                        return true;
                    }
					case Key::ENTER:
					case Key::KP_ENTER:
                    {
						if (on_enter_function && (input_event.type.flags & INPUT_EVENT_FLAG_SHIFT) == 0)
                        {
                            on_enter_function(shared_from_this());
                        }
                        else
                        {
                            cursor.string_begin = string.insert(cursor.string_begin, L'\n') + 1;
                            cursor.string_end = cursor.string_begin;

                            dirty();
                        }

                        return true;
                    }
					case Key::HOME:
                    {
                        // TODO: this is a bit inefficient as it requires iteration over all lines
                        //would be more expedient if we dealt with finding the line the cursor is
                        //on during some sort of cleaning phase?
                        for (auto& line : lines)
                        {
                            if (cursor.string_begin >= line.string_begin && cursor.string_begin <= line.string_end)
                            {
								if (input_event.type.flags == INPUT_EVENT_FLAG_SHIFT)
                                {
                                    cursor.string_end = cursor.string_begin;
                                    cursor.string_begin = line.string_begin;
                                }
                                else
                                {
                                    cursor.string_begin = line.string_begin;
                                    cursor.string_end = cursor.string_begin;
                                }

                                update_cursor();

                                return true;
                            }
                        }

                        break;
                    }
					case Key::END:
                    {
                        //TODO: this is a bit inefficient as it requires iteration over all lines
                        //would be more expedient if we dealt with finding the line the cursor is
                        //on during some sort of cleaning phase?
                        for (auto& line : lines)
                        {
                            if (cursor.string_begin >= line.string_begin && cursor.string_begin <= line.string_end)
                            {
								if (input_event.type.flags == INPUT_EVENT_FLAG_SHIFT)
                                {
                                    cursor.string_end = line.string_end;
                                }
                                else
                                {
                                    cursor.string_begin = line.string_end;
                                    cursor.string_end = cursor.string_begin;
                                }

                                update_cursor();

                                return true;
                            }
                        }

                        break;
                    }
					case Key::DEL:
                    {
                        if (cursor.string_begin != cursor.string_end)
                        {
                            cursor.string_begin = string.erase(cursor.string_begin, cursor.string_end);
                        }
                        else
                        {
                            cursor.string_begin = string.erase(cursor.string_begin);
                        }

                        cursor.string_end = cursor.string_begin;

                        dirty();

                        return true;
                    }
					case Key::LEFT:
                    {
                        if (cursor.string_begin != string.begin())
                        {
                            --cursor.string_begin;

							if (input_event.type.flags != INPUT_EVENT_FLAG_SHIFT)
                            {
                                cursor.string_end = cursor.string_begin;
                            }

                            update_cursor();
                        }

                        return true;
                    }
					case Key::RIGHT:
                    {
                        if (cursor.string_begin < string.end())
                        {
                            cursor.string_end = cursor.string_begin + 1;

							if (input_event.type.flags != INPUT_EVENT_FLAG_SHIFT)
                            {
                                cursor.string_begin = cursor.string_end;
                            }

                            update_cursor();
                        }

                        return true;
                    }
					case Key::DOWN:
                    {
                        //TODO: set cursor to the same column on the next line

                        break;
                    }
					case Key::UP:
                    {
                        //TODO: set cursor to the same column on the previous line

                        break;
                    }
					case Key::V:   //PASTE
						if (input_event.type.flags == INPUT_EVENT_FLAG_CTRL)
                        {
                            const auto clipboard_string = wstring_convert.from_bytes(platform.get_clipboard_string().c_str());
                            auto paste_length = clipboard_string.length();

                            if (max_length)
                            {
                                paste_length = std::min(paste_length, max_length.get() - string.length());
                            }

                            cursor.string_begin = string.insert(cursor.string_begin, clipboard_string.begin(), clipboard_string.begin() + paste_length) + paste_length;
                            cursor.string_end = cursor.string_begin;

                            dirty();

                            return true;
                        }
                        break;
					case Key::C:   //COPY
						if (input_event.type.flags == INPUT_EVENT_FLAG_CTRL)
                        {
                            StringType string = { cursor.string_begin, cursor.string_end };
                            platform.set_clipboard_string(wstring_convert.to_bytes(string.c_str()));

                            //TODO: 

                            return true;
                        }
                        break;
					case Key::X:   //CUT
                    {
						if (input_event.type.flags == INPUT_EVENT_FLAG_CTRL)
                        {
                            StringType string = { cursor.string_begin, cursor.string_end };
                            platform.set_clipboard_string(wstring_convert.to_bytes(string.c_str()));

                            cursor.string_begin = string.erase(cursor.string_begin, cursor.string_end);
                            cursor.string_end = cursor.string_begin;

                            dirty();

                            return true;
                        }
                        break;
                    }
					case Key::A:   //SELECT ALL
                    {
						if (input_event.type.flags == INPUT_EVENT_FLAG_CTRL)
                        {
                            cursor.string_begin = string.begin();
                            cursor.string_end = string.end();

                            update_cursor();

                            return true;
                        }
                        break;
                    }
                    default:
                        break;
                    }
                }
				else if (input_event.type.action == InputActionType::CHARACTER)
                {
					if (!max_length || max_length.get() > string.length())
					{
						if (cursor.string_begin != cursor.string_end)
						{
							cursor.string_begin = string.erase(cursor.string_begin, cursor.string_end);
						}

						cursor.string_begin = string.insert(cursor.string_begin, input_event.type.character);
						cursor.string_end = ++cursor.string_begin;

						dirty();
					}

                    return true;
                }
            }
        }

        return false;
    }

    void GUILabel::update_cursor()
    {
        for (const auto& line : lines)
        {
            bool a = (cursor.string_begin >= line.string_begin);
            bool b = (cursor.string_begin <= line.string_end);
            bool is_cursor_on_line = (a && b);

            if (is_cursor_on_line)
            {
                cursor.rectangle.x = line.rectangle.x + bitmap_font->get_string_width({ line.string_begin, cursor.string_begin });
                cursor.rectangle.y = line.rectangle.y;
                cursor.rectangle.height = bitmap_font->get_line_height();
                cursor.rectangle.width = 1;

                break;
            }
        }

        cursor.time_point = cursor_data_t::clock_type::now();
    }

    void GUILabel::update_lines()
    {
        static const auto OBSCURE_CHARACTER = L'•';
        static const auto FALLBACK_CHARACTER = L'?';
        static const auto ELLIPSE_CHARACTER = L'.';
        static const auto ELLIPSES_MAX = 3;

        if (bitmap_font == nullptr)
        {
            throw std::exception();
        }

        auto string_itr = string.begin();
        const auto padded_size = (get_bounds() - get_padding()).size();

        lines.clear();

        while (string_itr != string.end())
        {
            if (!is_multiline && !is_autosized_to_text)
            {
                bool will_overflow = (lines.size() + 1) * get_line_height() > padded_size.y;

                if (!lines.empty())
                {
                    //adding another line would exceed maximum height or line count
                    auto& line_string = lines.back().render_string;

                    if (is_read_only && should_use_ellipses && !line_string.empty())
                    {
                        //attempt to add ellipses to the end of the last line
                        const auto ellipse_width = bitmap_font->get_characters().at(ELLIPSE_CHARACTER).advance_x;
                        const auto ellipse_count = std::min(static_cast<decltype(ellipse_width)>(padded_size.x) / ellipse_width, ELLIPSES_MAX);
                        auto width = 0;

                        auto string_reverse_itr = line_string.rbegin() + 1;

                        //travel backwards from the end and calculate what part of the string will be replaced with ellipses
                        while (string_reverse_itr != line_string.rend())
                        {
                            width += bitmap_font->get_characters().at(*string_reverse_itr).advance_x;

                            if (width >= (ellipse_count * ellipse_width))
                            {
                                break;
                            }

                            ++string_reverse_itr;
                        }

                        line_string.erase(string_reverse_itr.base() + 1, line_string.end());
                        line_string.append(ellipse_count, ELLIPSE_CHARACTER);
                    }
                }

                break;
            }

            line_t::width_type line_width = 0;
            auto string_begin = string_itr;
            auto string_end = string_itr;
            auto was_line_added = false;
            auto string_space_itr = string.end();

            auto parse_color_codes = [&](StringType& string, std::vector<std::pair<size_t, vec4>>& color_pushes, std::vector<size_t>& color_pops)
            {
                auto string_itr = string.begin();

                while (string_itr != string.end())
                {
                    if (*string_itr == color_push_character<StringType::value_type>::VALUE)
                    {
                        //encountered color push character
                        string_itr = string.erase(string_itr);

                        if (string_itr != string.end() && *string_itr != color_push_character<StringType::value_type>::VALUE)
                        {
                            //determine how many characters to erase
                            auto erase_count = std::min(std::distance(string_itr, string.end()), rgb_hex_string_length);

                            if (erase_count == rgb_hex_string_length)
                            {
                                //try to parse a hex color from string
                                StringType hex_string = { string_itr, string_itr + rgb_hex_string_length };

                                try
                                {
                                    auto color = vec4(hex_to_rgb(hex_string), 1);

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
                    else if (*string_itr == color_pop_character<StringType::value_type>::VALUE)
                    {
                        //encountered color pop character
                        string_itr = string.erase(string_itr);

                        if (string_itr == string.end() || *string_itr != color_pop_character<StringType::value_type>::VALUE)
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

            auto replace_unrecognized_characters = [&](StringType& string)
            {
                auto string_itr = string.begin();

                while (string_itr != string.end())
                {
                    if (get_bitmap_font()->get_characters().find(*string_itr) == get_bitmap_font()->get_characters().end())
                    {
                        string.replace(string_itr, string_itr + 1, 1, FALLBACK_CHARACTER);
                    }

                    ++string_itr;
                }
            };

			auto add_line = [&](StringIteratorType string_begin, StringIteratorType string_end)
            {
                line_t line;
                line.string_begin = string_begin;
                line.string_end = string_end;

                StringType render_string = { string_begin, string_end };

                //strip color codes from string
                if (should_use_color_codes)
                {
                    parse_color_codes(render_string, line.colors_pushes, line.color_pop_indices);
                }

                replace_unrecognized_characters(render_string);

                if (is_obscured)
                {
                    render_string.replace(render_string.begin(), render_string.end(), render_string.size(), OBSCURE_CHARACTER);
                }

                auto render_string_width = bitmap_font->get_string_width(render_string);

                line.render_string = render_string;
                line.rectangle.width = render_string_width;
                line.rectangle.height = bitmap_font->get_line_height();
                line.rectangle.x = 0;
                line.rectangle.y = static_cast<f32>(lines.size()) * -get_line_height();

                switch (justification)
                {
                case Justification::CENTER:
                    line.rectangle.x -= render_string_width / 2;
                    break;
                case Justification::RIGHT:
                    line.rectangle.x -= render_string_width;
                    break;
                default:
                    break;
                }

                lines.emplace_back(line);

                was_line_added = true;
            };

            while (string_itr != string.end())
            {
                if (*string_itr == L'\n' && is_multiline)
                {
                    add_line(string_begin, string_itr);

                    ++string_itr;

                    if (string_itr == string.end())
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

                i16 character_width = 0;

                if (should_use_color_codes)
                {
                    if (*string_itr == color_push_character<StringType::value_type>::VALUE)
                    {
                        //encountered color push character

                        //skip to next character
                        ++string_itr;

                        if (string_itr == string.end())
                        {
                            break;
                        }
                        else if (*string_itr != color_push_character<StringType::value_type>::VALUE)
                        {
                            string_itr += std::min(std::distance(string_itr, string.end()), rgb_hex_string_length);

                            continue;
                        }
                    }
                    else if (*string_itr == color_pop_character<StringType::value_type>::VALUE)
                    {
                        //encountered color pop character
                        ++string_itr;

                        if (string_itr == string.end())
                        {
                            break;
                        }
                        else if (*string_itr != color_pop_character<StringType::value_type>::VALUE)
                        {
                            continue;
                        }
                    }
                }

                u16 character_id = *string_itr;

                auto characters_itr = bitmap_font->get_characters().find(character_id);

                auto character = FALLBACK_CHARACTER;

                if (is_obscured)
                {
                    character = OBSCURE_CHARACTER;
                }
                else if (characters_itr != bitmap_font->get_characters().end())
                {
                    character = *string_itr;
                }

                character_width = bitmap_font->get_characters().at(character).advance_x;

                if (!is_autosized_to_text && is_multiline && line_width + character_width > padded_size.x)
                {
                    if (string_space_itr != string.end())
                    {
                        //text will overrun the width limit, end the line here and start a new one
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

        if (lines.empty())
        {
            //HACK: if no lines, add one in so that the cursor can draw on empty lines
            //TODO: consolidate this with the normal add_line function
            line_t line;
            line.string_begin = string.begin();
            line.string_end = string.end();
            line.rectangle.height = bitmap_font->get_line_height();

            lines.emplace_back(line);
        }
    }
}