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
    state::state()
    {
        layout = boost::make_shared<gui_layout>();
        layout->set_dock_mode(gui_dock_mode::FILL);
    }

    void state::tick(f32 dt)
    {
        on_tick(dt);

        //TODO: get child nodes to tell layout about cleanliness, recursing every tick is expensive!
        std::function<bool(const boost::shared_ptr<gui_node>&)> is_dirty = [&](const boost::shared_ptr<gui_node>& node)
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

    void state::render()
    {
        const auto screen_size = platform.get_screen_size();
        auto view_projection_matrix = glm::ortho(0.0f, static_cast<f32>(screen_size.x), 0.0f, static_cast<f32>(screen_size.y));

        auto depth_state = gpu.depth.get_state();
        depth_state.should_test = false;

        gpu.depth.push_state(depth_state);

        layout->render(mat4(), view_projection_matrix);

        gpu.depth.pop_state();
    }

    bool state::on_input_event(input_event_t& input_event)
    {
        return layout->on_input_event(input_event);
    }

    void state::on_enter()
    {
        layout->set_bounds(gui_node::bounds_type(vec2(), static_cast<vec2>(platform.get_screen_size())));
    }

#if defined(MANDALA_PC)
    void state::on_window_event(window_event& window_event)
    {
        if (window_event.type == window_event_type::RESIZE)
        {
            layout->set_bounds(gui_node::bounds_type(vec2(), static_cast<vec2>(window_event.rectangle.size())));
        }
    }
#endif
}
