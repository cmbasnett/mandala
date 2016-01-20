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
    struct bitmap_font;

    struct gui_label : gui_node
    {
        typedef std::wstring string_type;
        typedef string_type::iterator string_itr_type;
        typedef u16 line_height_type;

        enum class justification_e
        {
            LEFT,
            CENTER,
            RIGHT
        };

        enum class vertical_alignment_e
        {
            TOP,
            MIDDLE,
            BOTTOM
        };

        gui_label();

        static string_type& escape_string(string_type& string);
        static string_type& sanitize_string(string_type& string);

        const string_type& get_string() const { return string; }
        const boost::shared_ptr<bitmap_font>& get_bitmap_font() const { return bitmap_font; }
        justification_e get_justification() const { return justification; };
        vertical_alignment_e get_vertical_alignment() const { return vertical_alignment; }
        line_height_type get_line_spacing() const { return line_spacing; }
        bool get_is_multiline() const { return is_multiline; }
        bool get_should_use_ellipses() const { return should_use_ellipses; }
        bool get_should_use_color_codes() const { return should_use_color_codes; }
        bool get_is_read_only() const { return is_read_only; }
        boost::optional<size_t> get_max_length() const { return max_length; }
        bool get_is_autosized_to_text() const { return is_autosized_to_text; }
        bool get_is_obscured() const { return is_obscured; }

        line_height_type get_line_height() const;
        size_t get_line_count() const;

        void set_string(const string_type& string);
        void set_bitmap_font(const boost::shared_ptr<bitmap_font>& bitmap_font) { this->bitmap_font = bitmap_font; dirty(); }
        void set_justification(justification_e justification) { this->justification = justification; dirty(); }
        void set_vertical_alignment(vertical_alignment_e vertical_alignment) { this->vertical_alignment = vertical_alignment; dirty(); }
        void set_line_spacing(line_height_type line_spacing) { this->line_spacing = line_spacing; dirty(); }
        void set_is_multiline(bool is_multiline) { this->is_multiline = is_multiline; dirty(); }
        void set_should_use_ellipses(bool should_use_ellipses) { this->should_use_ellipses = should_use_ellipses; dirty(); }
        void set_should_use_color_codes(bool should_use_color_codes) { this->should_use_color_codes = should_use_color_codes; dirty(); }
        void set_is_read_only(bool is_read_only) { this->is_read_only = is_read_only; dirty(); }    //TODO: might not need to dirty, putting this in to be safe
        void set_max_length(const boost::optional<size_t>& max_length);
        void set_is_autosized_to_text(bool is_autosized_to_text) { this->is_autosized_to_text = is_autosized_to_text; dirty(); }
        void set_is_obscured(bool is_obscured) { this->is_obscured = is_obscured; dirty(); }

        virtual void on_clean_begin() override;
        virtual void on_clean_end() override;
        virtual void on_render_begin(mat4& world_matrix, mat4& view_projection_matrix) override;
        virtual bool on_input_event_begin(input_event_t& input_event) override;

    private:
        struct line_t
        {
            typedef i32 width_type;
            typedef vec4 color_type;
            typedef std::pair<size_t, color_type> color_push_type;

            string_itr_type string_begin;
            string_itr_type string_end;
            string_type render_string;
            rectangle rectangle;

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
            rectangle rectangle;
        };

        void update_cursor();
        void update_lines();

        string_type string;
        boost::shared_ptr<bitmap_font> bitmap_font;
        justification_e justification = justification_e::LEFT;
        vertical_alignment_e vertical_alignment = vertical_alignment_e::TOP;
        line_height_type line_spacing = 0;
        bool is_multiline = true;
        bool is_uppercase = false;
        bool should_use_ellipses = true;
        bool should_use_color_codes = true;
        cursor_data_t cursor;
        bool is_read_only = true;
        std::vector<line_t> lines;
        boost::optional<size_t> max_length;
        std::function<void()> on_enter_function;
        bool is_autosized_to_text = false;
        bool is_obscured = false;
    };
}
