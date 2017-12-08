#pragma once

//boost
#include <boost\function.hpp>

//naga
#include "gui_node.hpp"
#include "platform_defs.hpp"

namespace naga
{
	struct GUIButton : GUINode
    {
        enum class State
        {
            IDLE,
#if defined(NAGA_PC)
            HOVER,
#endif
            PRESSED
        };

		virtual bool on_input_event_begin(InputEvent& input_event) override;

        State get_state() const { return state; }

		boost::function<void(boost::shared_ptr<GUINode>&)> on_state_changed;
		boost::function<void(boost::shared_ptr<GUINode>&)> on_press;

    private:
		State state = State::IDLE;
    };
}
