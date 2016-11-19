//naga
#include "gui_scroll.hpp"
#include "input_event.hpp"
#include "collision.hpp"

//glm
#include <glm\gtc\matrix_transform.hpp>

namespace naga
{
    void gui_scroll::on_render_begin(mat4& world_matrix, mat4& view_projection_matrix)
    {
        world_matrix = glm::translate(world_matrix, vec3(scroll_location, 0));
    }

    bool gui_scroll::on_input_event_begin(input_event_t& input_event)
    {
        //TODO: have some way for layouts to keep track of certain nodes
        //'owning' touch events by id so that they get exclusive rights
        //to handle future touch events of the same id.

        if (input_event.device_type == input_event_t::device_type_e::TOUCH)
        {
            //offset the touch location by the scroll location so that
            //touch locations are relative to the internal contents
            //of the scroll.
            input_event.touch.location -= scroll_location;
        }

        auto base_result = gui_node::on_input_event_begin(input_event);
        
        if (input_event.device_type == input_event_t::device_type_e::TOUCH)
        {
            //undo the scroll offset.
            input_event.touch.location += scroll_location;
        }

        if (base_result)
        {
            //input event was handled by this node's children, bail out.
            return true;
        }

        switch (input_event.touch.type)
        {
        case input_event_t::touch_t::type_e::PRESS:
            if (contains(get_bounds(), input_event.touch.location))
            {
                is_scrolling = true;
                touch_id = input_event.touch.id;
                return true;
            }
            break;
        case input_event_t::touch_t::type_e::RELEASE:
            if (is_scrolling && touch_id == input_event.touch.id)
            {
                is_scrolling = false;
                touch_id = 0;
                return true;
            }
            break;
        case input_event_t::touch_t::type_e::MOVE:
            if (is_scrolling && touch_id == input_event.touch.id)
            {
                scroll_location_target += input_event.touch.location_delta;
                scroll_location_target = glm::clamp(scroll_location_target, scroll_extents.min, scroll_extents.max);
                return true;
            }
            break;
        case input_event_t::touch_t::type_e::SCROLL:
            if (contains(get_bounds(), input_event.touch.location))
            {
                scroll_location_target += (input_event.touch.location_delta * 32.0);
                scroll_location_target = glm::clamp(scroll_location_target, scroll_extents.min, scroll_extents.max);
                return true;
            }
            break;
        }

        return false;
    }

    void gui_scroll::on_tick_end(f32 dt)
    {
        //TODO: figure out a proper algorithm for this smoothing
        scroll_location += (scroll_location_target - scroll_location) * dt * 30.0f;
        scroll_location = scroll_location_target;
        scroll_location = glm::clamp(scroll_location, scroll_extents.min, scroll_extents.max);
    }

    void gui_scroll::set_scroll_location(const scroll_location_type& scroll_location, bool should_interpolate)
    {
        scroll_location_target = scroll_location;

        if (!should_interpolate)
        {
            this->scroll_location = scroll_location;
        }
    }

    void gui_scroll::on_clean_end()
    {
        aabb2 scroll_extents;
        const auto bounds = get_bounds();

        for (const auto& child : get_children())
        {
            const auto child_bounds = child->get_bounds();

            scroll_extents.min = glm::min(scroll_extents.min, child_bounds.min - bounds.min);
            scroll_extents.max = glm::max(scroll_extents.max, child_bounds.max - bounds.max);
        }

        //HACK: figure out more elegant way to do this
        std::swap(scroll_extents.min, scroll_extents.max);

        scroll_extents.min *= -1;
        scroll_extents.max *= -1;

        this->scroll_extents = scroll_extents;
    }
}
