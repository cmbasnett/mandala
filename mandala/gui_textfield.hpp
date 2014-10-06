#pragma once

//mandala
#include "gui_label.hpp"

namespace mandala
{
    struct bitmap_font_t;

    struct gui_textfield_t : gui_label_t
    {
        struct cursor_t
        {
            size_t row_index = 0;
            size_t column_index = 0;
        };

        virtual void on_input_event(input_event_t& input_event) override;

        const string_type& placeholder_string() const { return _placeholder_string; }

        void set_placeholder_string(const string_type& placeholder_string) { _placeholder_string = placeholder_string; _is_dirty = true; }

    private:
        bool has_focus = true;
        cursor_t cursor;
        string_type _placeholder_string;
        string_type::value_type _cursor_character = L'_';
    };
}
