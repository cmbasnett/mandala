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
        typedef uint16_t line_height_type;

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

		const string_type& get_string() const { return string; }
		string_type get_string_escaped() const;
		string_type get_string_sanitized() const;
		const std::shared_ptr<bitmap_font_t>& get_bitmap_font() const { return bitmap_font; }
		justification_e get_justification() const { return justification; };
		vertical_alignment_e get_vertical_alignment() const { return vertical_alignment; }
        line_height_type get_line_spacing() const { return line_spacing; }
		bool get_is_multiline() const { return is_multiline; }
		bool get_should_use_ellipses() const { return should_use_ellipses; }
		bool get_should_use_color_codes() const { return should_use_color_codes; }
		bool get_is_read_only() const { return is_read_only; }

        line_height_type get_line_height() const;
		size_t get_line_count() const;

		void set_string(const string_type& string) { this->string = string; cursor.string_begin = this->string.begin(), cursor.string_end = this->string.begin(); dirty(); }
		void set_bitmap_font(std::shared_ptr<bitmap_font_t> bitmap_font) { this->bitmap_font = bitmap_font; dirty(); }
		void set_justification(justification_e justification) { this->justification = justification; dirty(); }
		void set_vertical_alignment(vertical_alignment_e vertical_alignment) { this->vertical_alignment = vertical_alignment; dirty(); }
        void set_line_spacing(line_height_type line_spacing) { this->line_spacing = line_spacing; dirty(); }
		void set_is_multiline(bool is_multiline) { this->is_multiline = is_multiline; dirty(); }
		void set_should_use_ellipses(bool should_use_ellipses) { this->should_use_ellipses = should_use_ellipses; dirty(); }
		void set_should_use_color_codes(bool should_use_color_codes) { this->should_use_color_codes = should_use_color_codes; dirty(); }
		void set_is_read_only(bool is_read_only) { this->is_read_only = is_read_only; dirty(); }	//TODO: might not need to dirty, putting this in to be safe

        virtual void clean() override;
        virtual void on_render_begin(const mat4_t& world_matrix, const mat4_t& view_projection_matrix) override;
        virtual void on_input_event(input_event_t& input_event) override;

    private:
		struct line_t
		{
			typedef int32_t width_type;
			typedef rgba_type color_type;
			typedef std::pair<size_t, color_type> color_push_type;

			string_itr_type string_begin;
			string_itr_type string_end;
			string_type render_string;  //TODO: have a central render string
            rectangle_t rectangle;

			std::vector<color_push_type> colors_pushes;
			std::vector<size_t> color_pop_indices;
		};

		struct cursor_data_t
		{
			typedef std::chrono::system_clock clock_type;
			typedef clock_type::time_point time_point_type;

			string_itr_type string_begin;
			string_itr_type string_end;
			time_point_type time_point;
            rectangle_t rectangle;
		};

        void update_cursor();

		string_type string;
		std::shared_ptr<bitmap_font_t> bitmap_font;
		justification_e justification = justification_e::left;
		vertical_alignment_e vertical_alignment = vertical_alignment_e::top;
        line_height_type line_spacing = 0;
		bool is_multiline = true;
		bool is_uppercase = false;
		bool should_use_ellipses = true;
		bool should_use_color_codes = true;
		cursor_data_t cursor;
		bool is_read_only = true;
		std::vector<line_t> lines;
	};
}
