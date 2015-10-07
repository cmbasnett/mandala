#pragma once

//boost
#include <boost\function.hpp>

//mandala
#include "gui_node.hpp"
#include "platform_defs.hpp"

namespace mandala
{
    struct gui_button : gui_node
    {
        enum class state_t
        {
            IDLE,
#if defined(MANDALA_PC)
            HOVER,
#endif
            PRESSED
        };

        virtual bool on_input_event_begin(input_event_t& input_event) override;

        state_t get_state() const { return state; }

        boost::function<void(boost::shared_ptr<gui_node>&)> on_state_changed;

    private:
        state_t state = state_t::IDLE;
    };
}
