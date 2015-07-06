#pragma once

//boost
#include <boost\function.hpp>

//mandala
#include "gui_node.hpp"
#include "platform_defs.hpp"

namespace mandala
{
    struct gui_button_t : gui_node_t
    {
        enum class state_t
        {
            IDLE,
#if defined(MANDALA_PC)
            HOVER,
#endif
            PRESSED
        };

        virtual bool on_input_event(input_event_t& input_event) override;

        state_t get_state() const { return state; }

#if defined(MANDALA_PC)
        boost::function<void()> on_hover;
        boost::function<void()> on_unhover;
#endif
        boost::function<void()> on_pressed;
    
    private:
        state_t state = state_t::IDLE;
    };
}
