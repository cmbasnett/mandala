#include "mouse.h"

namespace mandala
{
	mouse_t::state_t::state_t() :
		button_flags(0)
	{
	}


	bool mouse_t::state_t::is_down(button_t button) const
	{
		return (button_flags & (1 << static_cast<int32_t>(button))) != 0;
	}


	bool mouse_t::state_t::is_up(button_t button) const
	{
		return (button_flags & (1 << static_cast<int32_t>(button))) == 0;
	}

	mouse_t::mouse_t(GLFWwindow* window) :
		window(window)
	{
		glfwSetScrollCallback(window, &on_scroll);
		glfwSetKeyCallback(window, &on_key);
		glfwSetMouseButtonCallback(window, &on_button);
	}


	bool mouse_t::is_down(button_t button) const
	{
		return current.is_down(button);
	}


	bool mouse_t::is_up(button_t button) const
	{
		return current.is_up(button);
	}


	bool mouse_t::is_pressed(button_t button) const
	{
		return current.is_down(button) && previous.is_up(button);
	}


	bool mouse_t::is_released(button_t button) const
	{
		return current.is_up(button) && previous.is_down(button);
	}


	void mouse_t::tick()
	{
		previous = current;
	
		float64_t xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		current.button_flags = 0;
		current.location.x = static_cast<float32_t>(xpos);
		current.location.y = static_cast<float32_t>(ypos);

		for(int32_t i = GLFW_MOUSE_BUTTON_1; i <= GLFW_MOUSE_BUTTON_LAST; ++i)
			current.button_flags |= (glfwGetMouseButton(window, i) == GLFW_PRESS) ? 1 << i : 0;
	}


	void mouse_t::on_scroll(GLFWwindow* window, double x, double y)
	{
	}


	void mouse_t::on_key(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
	}

	
	void mouse_t::on_button(GLFWwindow* window, int button, int action, int mods)
	{
	}
};