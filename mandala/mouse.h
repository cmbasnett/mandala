#pragma once

#include "mandala.h"

namespace mandala
{
	struct mouse_t
	{
		enum class button_t : uint8_t
		{
			left	= GLFW_MOUSE_BUTTON_1,
			right	= GLFW_MOUSE_BUTTON_2,
			middle	= GLFW_MOUSE_BUTTON_3,
			four	= GLFW_MOUSE_BUTTON_4,
			five	= GLFW_MOUSE_BUTTON_5,
			six		= GLFW_MOUSE_BUTTON_6,
			seven	= GLFW_MOUSE_BUTTON_7,
			eight	= GLFW_MOUSE_BUTTON_8,
			count	= GLFW_MOUSE_BUTTON_LAST
		};

		struct state_t
		{
			state_t();

			bool is_up(button_t button) const;
			bool is_down(button_t button) const;

			vec2_t location;
			uint8_t button_flags;
			int32_t wheel;
		};

		mouse_t(GLFWwindow* window);

		static void on_scroll(GLFWwindow* window, double x, double y);
		static void on_key(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void on_button(GLFWwindow* window, int button, int action, int mods);

		bool is_down(button_t button) const;
		bool is_up(button_t button) const;
		bool is_pressed(button_t button) const;
		bool is_released(button_t button) const;
		vec2_t location() const { return current.location; }
		vec2_t location_delta() const { return current.location - previous.location; }

		void tick();
		
		GLFWwindow* window;
		state_t previous;
		state_t current;
	};
};