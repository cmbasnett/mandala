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

		const string_type& string() const { return _string; }
		const std::shared_ptr<bitmap_font_t>& bitmap_font() const { return _bitmap_font; }
		const justification_e justification() const { return _justification; };
		const vertical_alignment_e vertical_alignment() const { return _vertical_alignment; }
		bool is_multiline() const { return _is_multiline; }
		bool should_use_ellipses() const { return _should_use_ellipses; }
		bool should_use_color_codes() const { return _should_use_color_codes; }

		void set_string(const string_type& string) { _string = string; _is_dirty = true; }
		void set_bitmap_font(std::shared_ptr<bitmap_font_t> bitmap_font) { _bitmap_font = bitmap_font; _is_dirty = true; }
		void set_justification(justification_e justification) { _justification = justification; _is_dirty = true; }
		void set_vertical_alignment(vertical_alignment_e vertical_alignment) { _vertical_alignment = vertical_alignment; _is_dirty = true; }
		void set_is_multiline(bool is_multiline) { _is_multiline = is_multiline; _is_dirty = true; }
		void set_should_use_ellipses(bool should_use_ellipses) { _should_use_ellipses = should_use_ellipses; _is_dirty = true; }
		void set_should_use_color_codes(bool should_use_color_codes) { _should_use_color_codes = should_use_color_codes; _is_dirty = true; }

		virtual void clean() override;
        virtual void render(mat4_t world_matrix, mat4_t view_projection_matrix) override;

	private:
		struct render_data_t
		{
			typedef vec4_t color_type;

			struct line_t
			{
				typedef int32_t width_type;

				string_type string;
				width_type width = 0;
				std::vector<std::pair<size_t, color_type>> colors_pushes;
				std::vector<size_t> color_pop_indices;
			};

			std::vector<line_t> lines;
			vec2_t base_translation;
		};

		string_type _string;
		std::shared_ptr<bitmap_font_t> _bitmap_font;
		justification_e _justification = justification_e::left;
		vertical_alignment_e _vertical_alignment = vertical_alignment_e::top;
		bool _is_multiline = true;
		bool _should_use_ellipses = true;
		bool _should_use_color_codes = true;
		render_data_t _render_data;
	};
}
