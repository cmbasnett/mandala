#pragma once

//std
#include <sstream>

//glm
#include <glm\glm.hpp>
#include <glm\ext.hpp>

//boost
#include <boost\algorithm\string.hpp>

//mandala
#include "app.hpp"
#include "bitmap_font.hpp"
#include "gui_label.hpp"
#include "color.hpp"

namespace mandala
{
    void gui_label_t::render_data_t::reset()
    {
        lines.clear();
        base_translation = vec2_t(0);
    }

    gui_label_t::line_height_type gui_label_t::get_line_height() const
    {
        return _bitmap_font->line_height + _line_spacing;
    }

	void gui_label_t::clean()
    {
        static const auto color_push_character = L'↑';  //alt+24
        static const auto color_pop_character = L'↓';   //alt+25
        static const auto fallback_character = L'?';
        static const auto ellipse_character = L'.';
        static const auto ellipses_max = 3;

		gui_node_t::clean();

        _render_data.reset();

		if (_bitmap_font == nullptr)
		{
			throw std::exception();
		}

		auto string_itr = _string.begin();
        const auto padded_size = (bounds() - padding()).size();
        const auto line_height = get_line_height();

		while (string_itr != _string.end())
		{
            if ((_render_data.lines.size() + 1) * line_height > padded_size.y || (!_is_multiline && !_render_data.lines.empty()))
			{
				//adding another line would exceed maximum height or line count

				if (_should_use_ellipses && !_render_data.lines.empty() && !_render_data.lines.back().string.empty())
				{
					//attempt to add ellipses to the end of the last line
					const auto ellipse_width = _bitmap_font->characters.at(ellipse_character).advance_x;
					const auto ellipse_count = glm::min(static_cast<decltype(ellipse_width)>(padded_size.x) / ellipse_width, ellipses_max);

					auto& line_string = _render_data.lines.back().string;
					auto width = 0;

					auto string_reverse_itr = line_string.rbegin() + 1;

					//travel backwards from the end and calculate what part of the string will be replaced with ellipses
					while (string_reverse_itr != line_string.rend())
					{
						width += _bitmap_font->characters.at(*string_reverse_itr).advance_x;

						if (width >= (ellipse_count * ellipse_width))
						{
							break;
						}

						++string_reverse_itr;
					}

					line_string.erase(string_reverse_itr.base() + 1, line_string.end());
					line_string.append(ellipse_count, ellipse_character);
				}

				break;
			}

			render_data_t::line_t::width_type line_width = 0;
			auto string_begin = string_itr;
			auto string_end = string_itr;
			auto was_line_added = false;
			auto string_space_itr = _string.end();

			auto parse_color_codes = [&](string_type& string, std::vector<std::pair<size_t, vec4_t>>& color_pushes, std::vector<size_t>& color_pops)
			{
				auto string_itr = string.begin();

				while (string_itr != string.end())
				{
					if (*string_itr == color_push_character)
					{
						//encountered push character
						string_itr = string.erase(string_itr);

						if (string_itr != string.end() && *string_itr != color_push_character)
						{
							auto erase_count = std::min(std::distance(string_itr, string.end()), 6);

							if (erase_count >= 6)
							{
								//try to parse a hex color from string
								string_type hex_string = { string_itr, string_itr + 6 };

								try
								{
									auto color = vec4_t(hex_to_rgb(hex_string), 1.0f);

									color_pushes.push_back(std::make_pair(std::distance(string.begin(), string_itr), color));
								}
								catch (...)
								{
								}
							}

							string_itr = string.erase(string_itr, string_itr + erase_count);

							continue;
						}
					}
					else if (*string_itr == color_pop_character)
					{
						color_pops.push_back(std::distance(string.begin(), string_itr));

						//encountered pop character
						string_itr = string.erase(string_itr);

						continue;
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
					if (_bitmap_font->characters.find(*string_itr) == _bitmap_font->characters.end())
					{
						string.replace(string_itr, string_itr + 1, 1, fallback_character);
					}

					++string_itr;
				}
			};

			auto add_line = [&](string_type string)
			{
				render_data_t::line_t line;

				//strip color codes from string
				if (_should_use_color_codes)
				{
					parse_color_codes(string, line.colors_pushes, line.color_pop_indices);
				}

				replace_unrecognized_characters(string);

				line.string = string;
				line.width = _bitmap_font->get_string_width(string);

				_render_data.lines.emplace_back(line);

				was_line_added = true;
			};

			while (string_itr != _string.end())
			{
				if (*string_itr == L'\n')
				{
					add_line({ string_begin, string_itr });

					++string_itr;

					break;
				}
				else if (*string_itr == L' ' || *string_itr == L'-')
				{
					string_space_itr = string_itr;
				}

				int16_t character_width = 0;
				
				if (_should_use_color_codes)
				{
					if (*string_itr == color_push_character)
					{
						//encountered color push character

						//skip to next character
						++string_itr;

						if (string_itr == _string.end())
						{
							break;
						}
						else if (*string_itr != color_push_character)
						{
							string_itr += std::min(std::distance(string_itr, _string.end()), 6);

							continue;
						}
					}
					else if (*string_itr == color_pop_character)
					{
						//encountered color push character
						++string_itr;

						if (string_itr == _string.end())
						{
							break;
						}
						else if (*string_itr != color_pop_character)
						{
							continue;
						}
					}
				}

				uint16_t character_id = *string_itr;

				auto characters_itr = _bitmap_font->characters.find(character_id);

				string_type::value_type character;

				if (characters_itr == _bitmap_font->characters.end())
				{
					character = fallback_character;
				}
				else
				{
					character = *string_itr;
				}

				character_width = _bitmap_font->characters.at(character).advance_x;

				if (line_width + character_width > padded_size.x)
				{
					if (string_space_itr != _string.end())
					{
						add_line({ string_begin, string_space_itr });

						string_itr = string_space_itr + 1;
					}

					break;
				}

				line_width += character_width;

				++string_itr;
			}

			if (!was_line_added)
			{
				add_line({ string_begin, string_itr });
			}
		}

		_render_data.base_translation = bounds().min;
		_render_data.base_translation.x += padding().left;
		_render_data.base_translation.y += padding().bottom;

		const auto line_count = _render_data.lines.size();

		switch (_vertical_alignment)
		{
		case vertical_alignment_e::top:
			_render_data.base_translation.y += padded_size.y - _bitmap_font->base;
			break;
		case vertical_alignment_e::middle:
			_render_data.base_translation.y += (padded_size.y / 2) - (_bitmap_font->base / 2) + ((line_height * (line_count - 1)) / 2);
			break;
		case vertical_alignment_e::bottom:
			_render_data.base_translation.y += (line_height * line_count) - _bitmap_font->base;
			break;
		default:
			break;
		}

		switch (_justification)
		{
		case justification_e::center:
			_render_data.base_translation.x += padded_size.x / 2;
			break;
		case justification_e::right:
			_render_data.base_translation.x += padded_size.x;
			break;
		default:
			break;
		}
	}

    void gui_label_t::render(mat4_t world_matrix, mat4_t view_projection_matrix)
	{
		if (is_hidden())
		{
			return;
		}

        if (_bitmap_font == nullptr)
        {
            throw std::exception("bitmap font not set");
        }

        const auto line_height = get_line_height();
		auto base_translation = _render_data.base_translation;

		std::stack<vec4_t> color_stack;

		for (const auto& line : _render_data.lines)
		{
			auto translation = base_translation;

			switch (_justification)
			{
			case justification_e::center:
				translation.x -= line.width / 2;
				break;
			case justification_e::right:
				translation.x -= line.width;
				break;
			default:
				break;
			}

            const auto line_world_matrix = world_matrix * glm::translate(translation.x, translation.y, 0.0f);

			_bitmap_font->render_string(line.string, line_world_matrix, view_projection_matrix, color(), color_stack, line.colors_pushes, line.color_pop_indices);

            base_translation.y -= line_height;
        }

        gui_node_t::render(world_matrix, view_projection_matrix);
    }
}
