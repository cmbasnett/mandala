#pragma once

//std
#include <string>
#include <memory>
#include <vector>

//mandala
#include "gui_node.hpp"

namespace mandala
{
	struct bitmap_font_t;

	struct gui_label_t : gui_node_t
	{
		typedef std::wstring string_type;

		enum class justification_e : uint8_t
		{
			left,
			center,
			right
		};

		enum class vertical_alignment_e : uint8_t
		{
			top,
			middle,
			bottom
		};

		struct gradient_t
		{
			vec4_t color1 = vec4_t(1);
			vec4_t color2 = vec4_t(1);
		};

        string_type string;
		std::shared_ptr<bitmap_font_t> bitmap_font;
		justification_e justification = justification_e::left;
		vertical_alignment_e vertical_alignment = vertical_alignment_e::top;
		bool is_multiline = true;
		bool should_use_ellipses = true;
		bool should_use_gradient = false;
		bool should_use_color_codes = true;
		gradient_t gradient;

		virtual bool clean();
        virtual void render(mat4_t world_matrix, mat4_t view_projection_matrix) override;

	private:
		struct render_info_t
		{
			typedef vec4_t color_type;

			struct line_t
			{
				typedef int32_t width_type;

				line_t() :
					width(0)
				{
				}

				string_type string;
				width_type width = 0;
				std::vector<std::pair<size_t, color_type>> colors_pushes;
				std::vector<size_t> color_pop_indices;
			};

			std::vector<line_t> lines;
			vec2_t base_translation;
		};

		render_info_t render_info;
	};
}
