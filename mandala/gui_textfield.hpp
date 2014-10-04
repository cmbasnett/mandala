#pragma once

//mandala
#include "gui_label.hpp"

namespace mandala
{
    struct bitmap_font_t;

    struct gui_textfield_t : gui_label_t
    {
        struct caret_t
        {
            size_t row_index = 0;
            size_t column_index = 0;
        };

        virtual void on_input_event(input_event_t& input_event) override;

    private:
        bool has_focus = true;
        caret_t caret;
    };
}
