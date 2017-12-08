#pragma once

//std
#include <type_traits>
#include <exception>

namespace naga
{
    enum class Axis
    {
        X,
        Y,
        Z
    };

    enum class GUISizeMode
    {
        ABSOLUTE,
        RELATIVE,       // Size value is interpreted as a percentage (0.0-1.0) of the parent node's size
        AXIS_SCALE,     // Size value is interpreted as a percentage (0.0-1.0) of the opposing axis (eg. if X = 0.75, X will be 75% the size of Y)
        INHERIT
    };

    //TODO: make this more generic!
    struct GUISizeModes
    {
        GUISizeModes(GUISizeMode x = GUISizeMode::ABSOLUTE, GUISizeMode y = GUISizeMode::ABSOLUTE)
        {
            set_x(x);
            set_y(y);
        }

        GUISizeMode get_x() const { return x; }
        GUISizeMode get_y() const { return y; }

        void set_x(GUISizeMode x)
        {
            this->x = x;
        }

        void set_y(GUISizeMode y)
        {
            this->y = y;
        }

    private:
        GUISizeMode x = GUISizeMode::ABSOLUTE;
        GUISizeMode y = GUISizeMode::ABSOLUTE;
    };
}