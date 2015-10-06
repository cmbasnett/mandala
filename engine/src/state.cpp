//boost
#include <boost/make_shared.hpp>

//glm
#include "glm\ext.hpp"

//mandala
#include "opengl.hpp"
#include "state.hpp"
#include "platform.hpp"
#include "gpu.hpp"
#include "gui_layout.hpp"

#if defined(MANDALA_PC)
#include "window_event.hpp"
#endif

#include "line_renderer.hpp"
#include "interpolation.hpp"

namespace mandala
{
    state_t::state_t()
    {
        layout = boost::make_shared<gui_layout_t>();
        layout->set_dock_mode(gui_dock_mode_e::FILL);
    }

    void state_t::tick(float32_t dt)
    {
        on_tick(dt);

        //TODO: get child nodes to tell layout about cleanliness, recursing every tick is expensive!
        std::function<bool(const boost::shared_ptr<gui_node_t>&)> is_dirty = [&](const boost::shared_ptr<gui_node_t>& node)
        {
            if (node->get_is_dirty())
            {
                return true;
            }

            for (auto& child : node->get_children())
            {
                if (is_dirty(child))
                {
                    return true;
                }
            }

            return false;
        };

        if (is_dirty(layout))
        {
            layout->clean();
        }

        layout->tick(dt);
    }

    void state_t::render()
    {
        const auto screen_size = platform.get_screen_size();
        auto view_projection_matrix = glm::ortho(0.0f, static_cast<float32_t>(screen_size.x), 0.0f, static_cast<float32_t>(screen_size.y));

        auto depth_state = gpu.depth.get_state();
        depth_state.should_test = false;

        gpu.depth.push_state(depth_state);

        layout->render(mat4_t(), view_projection_matrix);

        gpu.depth.pop_state();
    }

    bool state_t::on_input_event(input_event_t& input_event)
    {
        return layout->on_input_event(input_event);
    }

    void state_t::on_enter()
    {
        layout->set_bounds(gui_node_t::bounds_type(vec2_t(), static_cast<vec2_t>(platform.get_screen_size())));
    }

#if defined(MANDALA_PC)
    void state_t::on_window_event(window_event_t& window_event)
    {
        if (window_event.type == window_event_t::type_e::RESIZE)
        {
            layout->set_bounds(gui_node_t::bounds_type(vec2_t(), static_cast<vec2_t>(window_event.rectangle.size())));
        }
    }
#endif
}
