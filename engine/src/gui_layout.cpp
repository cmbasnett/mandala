//naga
#include "gui_layout.hpp"
#include "input_event.hpp"
#include "platform.hpp"

namespace naga
{
    bool GUILayout::on_input_event_begin(InputEvent& input_event)
    {
		if (input_event.device_type == InputEvent::DeviceType::TOUCH)
        {
            input_event.touch.location.y = platform.get_screen_size().y - input_event.touch.location.y;

            auto touch_nodes_itr = touch_nodes.find(input_event.touch.id);

            if (touch_nodes_itr != touch_nodes.end())
            {
                if (touch_nodes_itr->second.expired())
                {
                    touch_nodes.erase(touch_nodes_itr);
                }
                else
                {
                    //pass input event to owning node to handle
                    touch_nodes_itr->second.lock()->on_input_event(input_event);
                }
            }
        }

        return false;
    }
}
