//glm
#include "glm\ext.hpp"

//mandala
#include "opengl.hpp"
#include "gui_state.hpp"
#include "platform.hpp"
#include "gpu.hpp"
#if defined(_WIN32) || defined(WIN32)
#include "window_event.hpp"
#endif

namespace mandala
{
	gui_state_t::gui_state_t()
	{
		const auto screen_size = platform.get_screen_size();

		layout = std::make_shared<gui_layout_t>();
		layout->set_dock_mode(gui_dock_mode_e::fill);
		layout->set_bounds(gui_node_t::bounds_type(vec2_t(), vec2_t(static_cast<float32_t>(screen_size.x), static_cast<float32_t>(screen_size.y))));
    }

    gui_state_t::~gui_state_t()
    {
    }

	void gui_state_t::tick(float32_t dt)
	{
		//TODO: get child nodes to tell layout about cleanliness
		if (layout->is_dirty())
		{
			layout->clean();
		}

        layout->tick(dt);
	}

	void gui_state_t::render()
	{
		const auto screen_size = platform.get_screen_size();

		auto world_matrix = mat4_t();
		auto view_projection_matrix = glm::ortho(0.0f, static_cast<float32_t>(screen_size.x), 0.0f, static_cast<float32_t>(screen_size.y));

        auto depth_state = gpu.depth.get_state();
        depth_state.should_test = false;

        gpu.depth.push_state(depth_state);

		layout->render(world_matrix, view_projection_matrix);

		gpu.depth.pop_state();
	}

	void gui_state_t::on_input_event(input_event_t& input_event)
	{
        layout->on_input_event(input_event);
    }

    void gui_state_t::on_window_event(window_event_t& window_event)
    {
        layout->dirty();
    }
}
