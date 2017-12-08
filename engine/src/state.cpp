//boost
#include <boost/make_shared.hpp>

//glm
#include "glm\ext.hpp"

//naga
#include "opengl.hpp"
#include "state.hpp"
#include "platform.hpp"
#include "gpu.hpp"
#include "gui_layout.hpp"

#if defined(NAGA_PC)
#include "window_event.hpp"
#endif

namespace naga
{
	State::State()
    {
        layout = boost::make_shared<GUILayout>();
        layout->set_dock_mode(GUIDockMode::FILL);
    }

	void State::tick(f32 dt)
    {
        on_tick(dt);

        //TODO: get child nodes to tell layout about cleanliness, recursing every tick is expensive!
		std::function<bool(const boost::shared_ptr<GUINode>&)> is_dirty = [&](const boost::shared_ptr<GUINode>& node)
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

	void State::render()
    {
        const auto screen_size = platform.get_screen_size();
        auto view_projection_matrix = glm::ortho(0.0f, static_cast<f32>(screen_size.x), 0.0f, static_cast<f32>(screen_size.y));

        auto depth_state = gpu.depth.get_state();
        depth_state.should_test = false;

        gpu.depth.push_state(depth_state);

        layout->render(mat4(), view_projection_matrix);

        gpu.depth.pop_state();
    }

	bool State::on_input_event(InputEvent& input_event)
    {
        return layout->on_input_event(input_event);
    }

	void State::on_enter()
	{
		layout->set_bounds(GUINode::BoundsType(vec2(), static_cast<vec2>(platform.get_screen_size())));
    }

#if defined(NAGA_PC)
	void State::on_window_event(WindowEvent& window_event)
    {
        if (window_event.type == WindowEventType::RESIZE)
        {
			layout->set_bounds(GUINode::BoundsType(vec2(), static_cast<vec2>(window_event.rectangle.size())));
        }
    }
#endif
}
