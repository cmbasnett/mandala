//glm
#include "glm\ext.hpp"

//mandala
#include "opengl.hpp"
#include "gui_state.hpp"
#include "platform.hpp"
#include "gpu.hpp"

namespace mandala
{
	gui_state_t::gui_state_t()
	{
		const auto screen_size = platform.get_screen_size();

		layout = std::make_shared<gui_layout_t>();
		layout->dock_mode = gui_node_t::dock_mode_e::fill;
		layout->bounds.max.x = static_cast<float32_t>(screen_size.x);
		layout->bounds.max.y = static_cast<float32_t>(screen_size.y);
	}

	gui_state_t::~gui_state_t()
	{
	}

	void gui_state_t::render()
	{
		const auto screen_size = platform.get_screen_size();

		auto world_matrix = mat4_t();
		auto projection_matrix = glm::ortho(0.0f, static_cast<float32_t>(screen_size.x), 0.0f, static_cast<float32_t>(screen_size.y));

		auto gpu_depth_state = gpu.depth.top();
		gpu_depth_state.should_test = false;

		gpu.depth.push(gpu_depth_state);

		layout->render(world_matrix, projection_matrix);

		gpu.depth.pop();
	}
}
