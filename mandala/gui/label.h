#pragma once

//std
#include <string>
#include <memory>

//mandala
#include "../mandala.h"
#include "node.h"

namespace mandala
{
	struct bitmap_font_t;

	namespace gui
	{
		struct label_t : node_t
		{
			typedef std::wstring string_type_t;

			enum class justification_t : uint8_t
			{
				left,
				center,
				right
			};

			enum class vertical_alignment_t : uint8_t
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

			string_type_t string;
			std::shared_ptr<bitmap_font_t> bitmap_font;
			justification_t justification = justification_t::left;
			vertical_alignment_t vertical_alignment = vertical_alignment_t::top;
			bool is_multiline = true;
			bool should_use_ellipses = true;
			bool should_use_gradient = false;
			gradient_t gradient;

			virtual bool clean();
            virtual void render(mat4_t world_matrix, mat4_t view_projection_matrix) override;

		private:
			struct render_info_t
			{
				struct line_t
				{
					typedef int32_t width_type_t;

					line_t() :
						width(0)
					{
					}

					line_t(const string_type_t& string, width_type_t width) :
						string(string),
						width(width)
					{
					}

					line_t(string_type_t&& string, width_type_t width) :
						string(std::move(string)),
						width(width)
					{
					}

					string_type_t string;
					width_type_t width = 0;
				};

				std::vector<line_t> lines;
			};

			render_info_t render_info;
		};
	};
};
