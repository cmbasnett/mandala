//mandala
#include "gui_scroll.hpp"
#include "input_event.hpp"
#include "collision.hpp"

//glm
#include <glm\gtc\matrix_transform.hpp>

namespace mandala
{
    void gui_scroll_t::render(mat4_t world_matrix, mat4_t view_projection_matrix)
    {
        world_matrix *= glm::translate(world_matrix, vec3_t(scroll_location, 0));

        gui_node_t::render(world_matrix, view_projection_matrix);
    }

    void gui_scroll_t::on_input_event(input_event_t& input_event)
    {
        //TODO: have some way for layouts to keep track of certain nodes 'owning' touch events by id so that they get exclusive rights to handle future touch events of the same id
		if (input_event.device_type == input_event_t::device_type_e::touch)
		{
			input_event.touch.location -= scroll_location;
		}

		gui_node_t::on_input_event(input_event);

		if (input_event.is_consumed)
		{
			return;
		}
		
		if (input_event.device_type == input_event_t::device_type_e::touch)
		{
			input_event.touch.location += scroll_location;
		}

        switch (input_event.touch.type)
        {
        case input_event_t::touch_t::type_e::press:
            if (contains(get_bounds(), input_event.touch.location))
            {
                is_scrolling = true;
                touch_id = input_event.touch.id;

				input_event.is_consumed = true;
            }
            break;
        case input_event_t::touch_t::type_e::release:
            if (is_scrolling && touch_id == input_event.touch.id)
            {
                is_scrolling = false;
				touch_id = 0;

				input_event.is_consumed = true;
            }
            break;
        case input_event_t::touch_t::type_e::move:
            if (is_scrolling && touch_id == input_event.touch.id)
            {
                scroll_location.x += static_cast<float32_t>(input_event.touch.location_delta.x);
				scroll_location.y -= static_cast<float32_t>(input_event.touch.location_delta.y);

				input_event.is_consumed = true;
            }
            break;
		}
    }

    void gui_scroll_t::tick(float32_t dt)
    {
        scroll_location = glm::clamp(scroll_location, scroll_extents.min, scroll_extents.max);

        gui_node_t::tick(dt);
    }

	void gui_scroll_t::clean()
	{
		gui_node_t::clean();

		//TODO: automatically size scroll extents based on children?
		for (auto& child : get_children())
		{
		}
	}
}
