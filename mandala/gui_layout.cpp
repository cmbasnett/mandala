//mandala
#include "gui_layout.hpp"
#include "input_event.hpp"
#include "platform.hpp"

namespace mandala
{
    void gui_layout_t::on_input_event(input_event_t& input_event)
    {
        if (input_event.device_type == input_event_t::device_type_e::touch)
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

        gui_node_t::on_input_event(input_event);
    }
}
