#pragma once

//std
#include <string>
#include <memory>
#include <vector>

//mandala
#include "gui_node.hpp"
#include "gpu_vertices.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

namespace mandala
{
	struct bitmap_font_t;

	struct gui_label_t : gui_node_t
	{
		typedef std::wstring string_type;
		typedef string_type::iterator string_itr_type;
        typedef uint16_t line_height_type;  //TODO: have this type based on typedef in bitmap_font

		static const auto color_push_character = L'↑';  //alt+24
		static const auto color_pop_character = L'↓';   //alt+25

		enum class justification_e
		{
			left,
			center,
			right
		};

        enum class vertical_alignment_e
        {
            top,
            middle,
            bottom
        };

		gui_label_t();

		const string_type& get_string() const { return _string; }
		string_type get_string_escaped() const;
		string_type get_string_sanitized() const;
		const std::shared_ptr<bitmap_font_t>& get_bitmap_font() const { return _bitmap_font; }
		justification_e get_justification() const { return _justification; };
		vertical_alignment_e get_vertical_alignment() const { return _vertical_alignment; }
        line_height_type get_line_spacing() const { return _line_spacing; }
		bool is_multiline() const { return _is_multiline; }
		bool should_use_ellipses() const { return _should_use_ellipses; }
		bool should_use_color_codes() const { return _should_use_color_codes; }
		bool is_read_only() const { return _is_read_only; }

        line_height_type get_line_height() const;
		size_t get_line_count() const;
		const string_type& get_line(size_t index) const;

		void set_string(const string_type& string) { _string = string; _cursor.begin = _string.begin(), _cursor.end = _string.begin(); _is_dirty = true; }
		void set_bitmap_font(std::shared_ptr<bitmap_font_t> bitmap_font) { _bitmap_font = bitmap_font; _is_dirty = true; }
		void set_justification(justification_e justification) { _justification = justification; _is_dirty = true; }
		void set_vertical_alignment(vertical_alignment_e vertical_alignment) { _vertical_alignment = vertical_alignment; _is_dirty = true; }
        void set_line_spacing(line_height_type line_spacing) { _line_spacing = line_spacing; _is_dirty = true; }
		void set_is_multiline(bool is_multiline) { _is_multiline = is_multiline; _is_dirty = true; }
		void set_should_use_ellipses(bool should_use_ellipses) { _should_use_ellipses = should_use_ellipses; _is_dirty = true; }
		void set_should_use_color_codes(bool should_use_color_codes) { _should_use_color_codes = should_use_color_codes; _is_dirty = true; }
		void set_is_read_only(bool is_read_only) { _is_read_only = is_read_only; _is_dirty = true; }	//TODO: might not need to dirty, putting this in to be safe

        virtual void clean() override;
        virtual void render_override(mat4_t world_matrix, mat4_t view_projection_matrix) override;
        virtual void on_input_event(input_event_t& input_event) override;

    private:
		struct line_t
		{
			typedef int32_t width_type;
			typedef rgba_type color_type;
			typedef std::pair<size_t, color_type> color_push_type;

			string_itr_type string_begin;
			string_itr_type string_end;
			string_type render_string;
			width_type width = 0;

			std::vector<color_push_type> colors_pushes;
			std::vector<size_t> color_pop_indices;
		};

		struct cursor_data_t
		{
			string_itr_type begin;
			string_itr_type end;
		};

		string_type _string;
		std::shared_ptr<bitmap_font_t> _bitmap_font;
		justification_e _justification = justification_e::left;
		vertical_alignment_e _vertical_alignment = vertical_alignment_e::top;
        line_height_type _line_spacing = 0;
		bool _is_multiline = true;
		bool _is_uppercase = false;
		bool _should_use_ellipses = true;
		bool _should_use_color_codes = true;
		cursor_data_t _cursor;
		bool _is_read_only = true;
		std::vector<line_t> _lines;
		vec2_t _render_base_translation;	//TODO: if more precached render data is made, throw this back into a struct

		//TODO: only have one of these, doesn't need to be constructed per instance (better yet, make a line renderer!)
		std::shared_ptr<vertex_buffer_t<basic_gpu_vertex_t>> _cursor_vertex_buffer;
		std::shared_ptr<index_buffer_t<uint8_t>> _cursor_index_buffer;
	};
}
