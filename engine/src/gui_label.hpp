#pragma once

//std
#include <string>
#include <vector>

//boost
#include <boost\shared_ptr.hpp>

//naga
#include "gui_node.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

namespace naga
{
	struct BitmapFont;

	struct GUILabel : GUINode
    {
        typedef std::wstring StringType;
        typedef StringType::iterator StringIteratorType;
        typedef u16 LineHeightType;

        enum class Justification
        {
            LEFT,
            CENTER,
            RIGHT
        };

        enum class VerticalAlignment
        {
            TOP,
            MIDDLE,
            BOTTOM
        };

		GUILabel();

        static StringType& escape_string(StringType& string);
        static StringType& sanitize_string(StringType& string);

        const StringType& get_string() const { return string; }
		const boost::shared_ptr<BitmapFont>& get_bitmap_font() const { return bitmap_font; }
		Justification get_justification() const { return justification; };
		VerticalAlignment get_vertical_alignment() const { return vertical_alignment; }
        LineHeightType get_line_spacing() const { return line_spacing; }
        bool get_is_multiline() const { return is_multiline; }
        bool get_should_use_ellipses() const { return should_use_ellipses; }
        bool get_should_use_color_codes() const { return should_use_color_codes; }
        bool get_is_read_only() const { return is_read_only; }
        boost::optional<size_t> get_max_length() const { return max_length; }
        bool get_is_autosized_to_text() const { return is_autosized_to_text; }
        bool get_is_obscured() const { return is_obscured; }
		boost::function<void(boost::shared_ptr<GUINode>&)> get_on_enter_function() const { return on_enter_function; }

        LineHeightType get_line_height() const;
        size_t get_line_count() const;

        void set_string(const StringType& string);
		void set_bitmap_font(const boost::shared_ptr<BitmapFont>& bitmap_font) { this->bitmap_font = bitmap_font; dirty(); }
        void set_justification(Justification justification) { this->justification = justification; dirty(); }
        void set_vertical_alignment(VerticalAlignment vertical_alignment) { this->vertical_alignment = vertical_alignment; dirty(); }
        void set_line_spacing(LineHeightType line_spacing) { this->line_spacing = line_spacing; dirty(); }
        void set_is_multiline(bool is_multiline) { this->is_multiline = is_multiline; dirty(); }
        void set_should_use_ellipses(bool should_use_ellipses) { this->should_use_ellipses = should_use_ellipses; dirty(); }
        void set_should_use_color_codes(bool should_use_color_codes) { this->should_use_color_codes = should_use_color_codes; dirty(); }
        void set_is_read_only(bool is_read_only) { this->is_read_only = is_read_only; dirty(); }    //TODO: might not need to dirty, putting this in to be safe
        void set_max_length(const boost::optional<size_t>& max_length);
        void set_is_autosized_to_text(bool is_autosized_to_text) { this->is_autosized_to_text = is_autosized_to_text; dirty(); }
        void set_is_obscured(bool is_obscured) { this->is_obscured = is_obscured; dirty(); }
		void set_on_enter_function(boost::function<void(boost::shared_ptr<GUINode>&)> on_enter_function) { this->on_enter_function = on_enter_function; }

        virtual void on_clean_begin() override;
        virtual void on_clean_end() override;
        virtual void on_render_begin(mat4& world_matrix, mat4& view_projection_matrix) override;
        virtual bool on_input_event_begin(InputEvent& input_event) override;

    private:
        struct line_t
        {
            typedef i32 width_type;
            typedef vec4 color_type;
            typedef std::pair<size_t, color_type> color_push_type;

            StringIteratorType string_begin;
            StringIteratorType string_end;
            StringType render_string;
            Rectangle rectangle;

            std::vector<color_push_type> colors_pushes;
            std::vector<size_t> color_pop_indices;
        };

        struct cursor_data_t
        {
            typedef std::chrono::system_clock clock_type;
            typedef clock_type::time_point time_point_type;

            StringIteratorType string_begin;
            StringIteratorType string_end;
            time_point_type time_point;
			Rectangle rectangle;
        };

        void update_cursor();
        void update_lines();

        StringType string;
        boost::shared_ptr<BitmapFont> bitmap_font;
		Justification justification = Justification::LEFT;
		VerticalAlignment vertical_alignment = VerticalAlignment::TOP;
        LineHeightType line_spacing = 0;
        bool is_multiline = true;
        bool is_uppercase = false;
        bool should_use_ellipses = true;
        bool should_use_color_codes = true;
        cursor_data_t cursor;
        bool is_read_only = true;
        std::vector<line_t> lines;
        boost::optional<size_t> max_length;
        boost::function<void(boost::shared_ptr<GUINode>&)> on_enter_function;
        bool is_autosized_to_text = false;
        bool is_obscured = false;
    };
}
