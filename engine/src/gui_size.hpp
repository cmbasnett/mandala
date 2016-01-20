#pragma once

//std
#include <type_traits>
#include <exception>

namespace naga
{
    enum class axis_e
    {
        X,
        Y,
        Z
    };

    enum class gui_size_mode_e
    {
        ABSOLUTE,
        RELATIVE,       // Size value is interpreted as a percentage (0.0-1.0) of the parent node's size
        AXIS_SCALE,     // Size value is interpreted as a percentage (0.0-1.0) of the opposing axis (eg. if X = 0.75, X will be 75% the size of Y)
        INHERIT
    };

    //TODO: make this more generic!
    struct gui_size_modes_t
    {
        gui_size_modes_t(gui_size_mode_e x = gui_size_mode_e::ABSOLUTE, gui_size_mode_e y = gui_size_mode_e::ABSOLUTE)
        {
            set_x(x);
            set_y(y);
        }

        gui_size_mode_e get_x() const { return x; }
        gui_size_mode_e get_y() const { return y; }

        void set_x(gui_size_mode_e x)
        {
            this->x = x;
        }

        void set_y(gui_size_mode_e y)
        {
            this->y = y;
        }

    private:
        gui_size_mode_e x = gui_size_mode_e::ABSOLUTE;
        gui_size_mode_e y = gui_size_mode_e::ABSOLUTE;
    };
}