//glm
#include "glm\ext.hpp"

//mandala
#include "gui_state.hpp"
#include "platform.hpp"

namespace mandala
{
	gui_state_t::gui_state_t()
	{
		const auto screen_size = platform.get_screen_size();

		layout = std::make_shared<gui_layout_t>();
		layout->dock_mode = gui_node_t::dock_mode_t::fill;
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

		//disable depth testing
		glDisable(GL_DEPTH_TEST);

		layout->render(world_matrix, projection_matrix);

		glEnable(GL_DEPTH_TEST);
	}
};
