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
	}

	void gui_state_t::render()
	{
		const auto screen_size = platform.get_screen_size();

		auto world_matrix = mat4_t();
		auto view_projection_matrix = glm::ortho(0.0f, static_cast<float32_t>(screen_size.x), 0.0f, static_cast<float32_t>(screen_size.y));

		auto gpu_depth_state = gpu.depth.top();
		gpu_depth_state.should_test = false;

		gpu.depth.push(gpu_depth_state);

		layout->render(world_matrix, view_projection_matrix);

		gpu.depth.pop();
	}

	void gui_state_t::on_input_event(input_event_t& input_event)
	{
		if (input_event.device_type == input_event_t::device_type_e::touch &&
			input_event.touch.type == input_event_t::touch_t::type_e::button_press)
		{
			gui_node_t::trace_args_t trace_args;
			trace_args.circle.origin = input_event.touch.position;
			trace_args.circle.radius = 8.0f;

			gui_node_t::trace_result_t trace_result;

			auto did_hit = gui_node_t::trace(layout, trace_args, trace_result);

			std::cout << trace_result.nodes_hit.size() << std::endl;

			for (auto& node : trace_result.nodes_hit)
			{
				node->on_input_event(input_event);

				if (input_event.is_consumed)
				{
					break;
				}
			}
		}
	}

}
