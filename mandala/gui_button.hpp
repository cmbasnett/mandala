#pragma once

//mandala
#include "gui_node.hpp"

namespace mandala
{
    struct gui_button_t : gui_node_t
    {
        enum class state_t
        {
            idle,
            hover,
            pressed
        };

        virtual void on_input_event(input_event_t& input_event) override;

        state_t state() const { return _state; }

        std::function<void()> _hover_function;
        std::function<void()> _unhover_function;
        std::function<void()> _pressed_function;
    
    private:
        state_t _state = state_t::idle;
    };
}