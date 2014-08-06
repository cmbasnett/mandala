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
	bool gui_label_t::clean()
	{
		bool did_clean = gui_node_t::clean();

		//TODO: make a render_info.reset() function
		render_info.lines.clear();

		for (auto& line : render_info.lines)
		{
			line.colors_pushes.clear();
			line.color_pop_indices.clear();
		}

		if (bitmap_font == nullptr)
		{
			throw std::exception();
		}

		auto string_itr = string.begin();
		const auto padded_size = (bounds - padding).size();
		static const auto color_push_character = L'↑';
		static const auto color_pop_character = L'↓';
		static const auto fallback_character = L'?';

		while (string_itr != string.end())
		{
			if ((render_info.lines.size() + 1) * bitmap_font->line_height > padded_size.y || (!is_multiline && !render_info.lines.empty()))
			{
				//adding another line would exceed maximum height or line count

				if (should_use_ellipses && !render_info.lines.empty() && !render_info.lines.back().string.empty())
				{
					//attempt to add ellipses to the end of the last line
					static const auto ellipse_character = L'.';
					static const auto ellipses_max = 3;
					const auto ellipse_width = bitmap_font->characters.at(ellipse_character).advance_x;
					const auto ellipse_count = glm::min(static_cast<decltype(ellipse_width)>(padded_size.x) / ellipse_width, ellipses_max);

					auto& line_string = render_info.lines.back().string;
					auto width = 0;

					auto string_reverse_itr = line_string.rbegin() + 1;

					//travel backwards from the end and calculate what part of the string will be replaced with ellipses
					while (string_reverse_itr != line_string.rend())
					{
						width += bitmap_font->characters.at(*string_reverse_itr++).advance_x;

						if (width >= (ellipse_count * ellipse_width))
						{
							break;
						}
					}

					line_string.erase(string_reverse_itr.base() + 1, line_string.end());
					line_string.append(ellipse_count, ellipse_character);
				}

				break;
			}

			render_info_t::line_t::width_type line_width = 0;
			auto string_begin = string_itr;
			auto string_end = string_itr;
			auto was_line_added = false;
			auto string_space_itr = string.end();

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
					if (bitmap_font->characters.find(*string_itr) == bitmap_font->characters.end())
					{
						string.replace(string_itr, string_itr + 1, 1, fallback_character);
					}

					++string_itr;
				}
			};

			auto add_line = [&](string_type string)
			{
				render_info_t::line_t line;

				//strip color codes from string
				if (should_use_color_codes)
				{
					parse_color_codes(string, line.colors_pushes, line.color_pop_indices);
				}

				replace_unrecognized_characters(string);

				line.string = string;
				line.width = bitmap_font->get_string_width(string);

				render_info.lines.emplace_back(line);

				was_line_added = true;
			};

			while (string_itr != string.end())
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
				
				if (should_use_color_codes)
				{
					if (*string_itr == color_push_character)
					{
						//encountered color push character

						//skip to next character
						++string_itr;

						if (string_itr == string.end())
						{
							break;
						}
						else if (*string_itr != color_push_character)
						{
							string_itr += std::min(std::distance(string_itr, string.end()), 6);

							continue;
						}
					}
					else if (*string_itr == color_pop_character)
					{
						//encountered color push character
						++string_itr;

						if (string_itr == string.end())
						{
							break;
						}
						else if (*string_itr != color_pop_character)
						{
							continue;
						}
					}
				}

				auto characters_itr = bitmap_font->characters.find(*string_itr);

				string_type::value_type character;

				if (characters_itr == bitmap_font->characters.end())
				{
					character = fallback_character;
				}
				else
				{
					character = *string_itr;
				}

				character_width = bitmap_font->characters.at(character).advance_x;

				if (line_width + character_width > padded_size.x)
				{
					if (string_space_itr != string.end())
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

		render_info.base_translation = bounds.min;
		render_info.base_translation.x += padding.left;
		render_info.base_translation.y += padding.bottom;

		const auto line_count = render_info.lines.size();

		switch (vertical_alignment)
		{
		case vertical_alignment_e::top:
			render_info.base_translation.y += padded_size.y - bitmap_font->base;
			break;
		case vertical_alignment_e::middle:
			render_info.base_translation.y += (padded_size.y / 2) - (bitmap_font->base / 2) + ((bitmap_font->line_height * (line_count - 1)) / 2);
			break;
		case vertical_alignment_e::bottom:
			render_info.base_translation.y += (bitmap_font->line_height * line_count) - bitmap_font->base;
			break;
		default:
			break;
		}

		switch (justification)
		{
		case justification_e::center:
			render_info.base_translation.x += padded_size.x / 2;
			break;
		case justification_e::right:
			render_info.base_translation.x += padded_size.x;
			break;
		default:
			break;
		}

		return did_clean;
	}

    void gui_label_t::render(mat4_t world_matrix, mat4_t view_projection_matrix)
    {
		if (is_hidden)
		{
			return;
		}

        if (bitmap_font == nullptr)
        {
            throw std::exception();
        }

		auto base_translation = render_info.base_translation;

		std::stack<vec4_t> color_stack;

		for (const auto& line : render_info.lines)
		{
			auto translation = base_translation;

			switch (justification)
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
			const auto color_top = should_use_gradient ? gradient.color1 : color;
			const auto color_bottom = should_use_gradient ? gradient.color2 : color;

			bitmap_font->render_string(line.string, line_world_matrix, view_projection_matrix, color, color_stack, line.colors_pushes, line.color_pop_indices);

			base_translation.y -= bitmap_font->line_height;
        }

        gui_node_t::render(world_matrix, view_projection_matrix);
    }
}
