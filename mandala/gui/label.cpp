#pragma once

//mandala
#include "../app.hpp"
#include "../bitmap_font.hpp"
#include "label.hpp"

//std
#include <sstream>

//glm
#include <glm\glm.hpp>
#include <glm\ext.hpp>

//boost
#include <boost\algorithm\string.hpp>

namespace mandala
{
    namespace gui
    {
		bool label_t::clean()
		{
			bool did_clean = node_t::clean();

			render_info.lines.clear();

			if (bitmap_font == nullptr)
			{
				throw std::exception();
			}

			auto string_itr = string.begin();
			const auto padded_size = (bounds - padding).size();

			while (string_itr != string.end())
			{
				if ((render_info.lines.size() + 1) * bitmap_font->line_height > padded_size.y || (!is_multiline && !render_info.lines.empty()))
				{
					//adding another line would exceed maximum height or line count

					if (should_use_ellipses && !render_info.lines.empty() && !render_info.lines.back().string.empty())
					{
						//attempt to add ellipses to the end of the last line
						const auto ellipse_char = L'.';
						const auto ellipses_max = 3;
						const auto ellipse_width = bitmap_font->characters.at(ellipse_char).advance_x;
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
						line_string.append(ellipse_count, ellipse_char);
					}

					break;
				}

				render_info_t::line_t::width_type line_width = 0;
				auto string_begin = string_itr;
				auto string_end = string_itr;
				auto was_line_added = false;

				decltype(string_itr) string_space_itr = string.end();

				auto add_line = [&](string_type&& string)
				{
					auto width = bitmap_font->get_string_width(string);
					render_info.lines.emplace_back(std::forward<string_type>(string), width);
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

					auto character_width = bitmap_font->characters.at(*string_itr).advance_x;

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

			return did_clean;
		}

        void label_t::render(mat4_t world_matrix, mat4_t view_projection_matrix)
        {
			if (is_hidden)
			{
				return;
			}

            if (bitmap_font == nullptr)
            {
                throw std::exception();
            }

            //node translation
			auto base_translation = bounds.min;
			base_translation.x += padding.left;
			base_translation.y += padding.bottom;

			const auto line_count = render_info.lines.size();
			const auto padded_size = (bounds - padding).size();

            switch (vertical_alignment)
            {
            case vertical_alignment_t::top:
				base_translation.y += padded_size.y - bitmap_font->base;
                break;
			case vertical_alignment_t::middle:
				base_translation.y += (padded_size.y / 2) - (bitmap_font->base / 2) + ((bitmap_font->line_height * (line_count - 1)) / 2);
				break;
			case vertical_alignment_t::bottom:
				base_translation.y += (bitmap_font->line_height * line_count) - bitmap_font->base;
				break;
            default:
                break;
            }

            switch (justification)
            {
            case justification_t::center:
				base_translation.x += padded_size.x / 2;
                break;
            case justification_t::right:
				base_translation.x += padded_size.x;
                break;
            default:
                break;
            }

			for (const auto& line : render_info.lines)
			{
				auto translation = base_translation;

				switch (justification)
				{
				case justification_t::center:
					translation.x -= line.width / 2;
					break;
				case justification_t::right:
					translation.x -= line.width;
					break;
				default:
					break;
				}

                auto line_world_matrix = world_matrix * glm::translate(translation.x, translation.y, 0.0f);

				auto color_top = should_use_gradient ? gradient.color1 : color;
				auto color_bottom = should_use_gradient ? gradient.color2 : color;

				bitmap_font->render_string(line.string, color_top, color_bottom, line_world_matrix, view_projection_matrix);

				base_translation.y -= bitmap_font->line_height;
            }

            node_t::render(world_matrix, view_projection_matrix);
        }
    };
};
