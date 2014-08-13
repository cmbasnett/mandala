//std
#include <array>
#include <chrono>
#include <iostream>

//mandala
#include "opengl.hpp"
#include "platform_win32.hpp"

namespace mandala
{
    static inline void on_keyboard_key(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		input_event_t input_event;
		input_event.device_type = input_event_t::device_type_e::keyboard;
		input_event.keyboard.key = static_cast<input_event_t::keyboard_t::key_e>(key);

		if ((key & GLFW_MOD_SHIFT) != 0)
		{
			input_event.keyboard.mod_flags |= input_event_t::mod_flag_shift;
		}

        if ((key & GLFW_MOD_CONTROL) != 0)
        {
			input_event.keyboard.mod_flags |= input_event_t::mod_flag_ctrl;
        }

        if ((key & GLFW_MOD_ALT) != 0)
        {
			input_event.keyboard.mod_flags |= input_event_t::mod_flag_alt;
        }

        if ((key & GLFW_MOD_SUPER) != 0)
        {
			input_event.keyboard.mod_flags |= input_event_t::mod_flag_super;
        }

        switch (action)
        {
        case GLFW_RELEASE:
            input_event.keyboard.type = input_event_t::keyboard_t::type_e::key_release;
            break;
        case GLFW_PRESS:
            input_event.keyboard.type = input_event_t::keyboard_t::type_e::key_press;
            break;
        case GLFW_REPEAT:
            input_event.keyboard.type = input_event_t::keyboard_t::type_e::key_repeat;
            break;
        }

        platform.input.events.push_back(input_event);
    }

    static inline void on_keyboard_character(GLFWwindow* window, unsigned int character)
    {
        input_event_t input_event;
        input_event.device_type = input_event_t::device_type_e::keyboard;
        input_event.keyboard.type = input_event_t::keyboard_t::type_e::character;
        input_event.keyboard.character = character;

        platform.input.events.push_back(input_event);
    }

    static inline void on_mouse_button(GLFWwindow* window, int button, int action, int mods)
    {
        input_event_t input_event;
        input_event.device_type = input_event_t::device_type_e::touch;
        input_event.touch.button = static_cast<input_event_t::touch_t::button_e>(button);
		input_event.touch.mod_flags = mods;

        bool is_press = action == GLFW_PRESS;

        input_event.touch.type = is_press ? input_event_t::touch_t::type_e::button_press : input_event_t::touch_t::type_e::button_release;

        if (is_press)
        {
            input_event.touch.id = platform.input.touch_id++;
        }

        glfwGetCursorPos(window, &input_event.touch.position.x, &input_event.touch.position.y);

        platform.input.events.push_back(input_event);
    }

    static inline void on_mouse_move(GLFWwindow* window, double x, double y)
	{
		input_event_t input_event;
		input_event.device_type = input_event_t::device_type_e::touch;
		input_event.touch.type = input_event_t::touch_t::type_e::move;
		input_event.touch.position.x = x;
		input_event.touch.position.y = y;
		input_event.touch.position_delta.x = x - platform.cursor_position.x;
		input_event.touch.position_delta.y = y - platform.cursor_position.y;

		if (platform.is_cursor_centered)
		{
			const auto screen_size = platform.get_screen_size();

			platform.set_cursor_position(static_cast<vec2_f64_t>(screen_size) / 2.0);
		}

        platform.cursor_position = platform.get_cursor_position();

        platform.input.events.push_back(input_event);
    }

    static inline void on_mouse_scroll(GLFWwindow* window, double x, double y)
    {
        input_event_t input_event;
        input_event.device_type = input_event_t::device_type_e::touch;
        input_event.touch.type = input_event_t::touch_t::type_e::scroll;
        input_event.touch.position = platform.get_cursor_position();
        input_event.touch.position_delta.x = x;
        input_event.touch.position_delta.y = y;

        platform.input.events.push_back(input_event);
    }

    static inline void on_window_resize(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    static inline void on_error(int error_code, const char* message)
    {
        throw std::exception();
    }

    platform_win32_t platform;

    platform_win32_t::platform_win32_t()
    {
    }

	platform_win32_t::~platform_win32_t()
	{
	}

	void platform_win32_t::app_run_start()
	{
		//glfw
		glfwInit();

		glfwWindowHint(GLFW_SAMPLES, 16);

		window_ptr = glfwCreateWindow(1, 1, "mandala", nullptr, nullptr);

        glfwMakeContextCurrent(static_cast<GLFWwindow*>(window_ptr));

        glfwSetKeyCallback(static_cast<GLFWwindow*>(window_ptr), on_keyboard_key);
        glfwSetCharCallback(static_cast<GLFWwindow*>(window_ptr), on_keyboard_character);
        glfwSetMouseButtonCallback(static_cast<GLFWwindow*>(window_ptr), on_mouse_button);
        glfwSetCursorPosCallback(static_cast<GLFWwindow*>(window_ptr), on_mouse_move);
        glfwSetScrollCallback(static_cast<GLFWwindow*>(window_ptr), on_mouse_scroll);
        glfwSetWindowSizeCallback(static_cast<GLFWwindow*>(window_ptr), on_window_resize);

		//glew
		glewInit();
	}

	void platform_win32_t::app_run_end()
	{
		glfwTerminate();
	}

	void platform_win32_t::app_tick_start(float32_t dt)
	{
        glfwPollEvents();

		for (size_t gamepad_index = 0; gamepad_index < input.gamepad_states.size(); ++gamepad_index)
		{
            if (!glfwJoystickPresent(static_cast<int>(gamepad_index)))
			{
				break;
			}

			auto& gamepad_state = input.gamepad_states[gamepad_index];

			int axis_count = 0;
			auto axes = glfwGetJoystickAxes(static_cast<int>(gamepad_index), &axis_count);

			for (auto axis_index = 0; axis_index < axis_count; ++axis_index)
			{
				if (axes[axis_index] != gamepad_state.axes[axis_index])
				{
					input_event_t input_event;
					input_event.device_type = input_event_t::device_type_e::gamepad;
					input_event.gamepad.type = input_event_t::gamepad_t::type_e::axis_move;
					input_event.gamepad.axis_index = axis_index;
					input_event.gamepad.axis_value = axes[axis_index];
					input_event.gamepad.axis_value_delta = axes[axis_index] - gamepad_state.axes[axis_index];

					platform.input.events.push_back(input_event);
				}

				gamepad_state.axes[axis_index] = axes[axis_index];
			}

			int button_count = 0;
			auto buttons = glfwGetJoystickButtons(static_cast<int>(gamepad_index), &button_count);

			for (auto button_index = 0; button_index < button_count; ++button_index)
			{
				if (buttons[button_index] != gamepad_state.buttons[button_index])
				{
					input_event_t input_event;
					input_event.device_type = input_event_t::device_type_e::gamepad;
					input_event.gamepad.type = buttons[button_index] == 0 ? input_event_t::gamepad_t::type_e::button_release : input_event_t::gamepad_t::type_e::button_press;
					input_event.gamepad.button_index = button_index;

					platform.input.events.push_back(input_event);
				}

				gamepad_state.buttons[button_index] = buttons[button_index];
			}
		}
	}

	void platform_win32_t::app_tick_end(float32_t dt)
	{
	}

	void platform_win32_t::app_render_start()
	{
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void platform_win32_t::app_render_end()
	{
        glfwSwapBuffers(static_cast<GLFWwindow*>(window_ptr));
	}

	bool platform_win32_t::should_exit() const
	{
        return glfwWindowShouldClose(static_cast<GLFWwindow*>(window_ptr)) != 0;
	}

    vec2_i32_t platform_win32_t::get_screen_size() const
    {
		vec2_i32_t screen_size;

        glfwGetWindowSize(static_cast<GLFWwindow*>(window_ptr), &screen_size.x, &screen_size.y);

		return screen_size;
    }
	
	void platform_win32_t::set_screen_size(const vec2_i32_t& screen_size) const
	{
        glfwSetWindowSize(static_cast<GLFWwindow*>(window_ptr), screen_size.x, screen_size.y);
	}

    bool platform_win32_t::pop_input_event(input_event_t& input_event)
    {
		if (input.events.empty())
        {
            return false;
        }

		input_event = input.events.front();
		input_event.id = input.event_id;
		input_event.is_consumed = false;

		input.events.pop_front();

		++input.event_id;

        return true;
	}

	vec2_f64_t platform_win32_t::get_cursor_position() const
	{
		vec2_f64_t cursor_position;

        glfwGetCursorPos(static_cast<GLFWwindow*>(window_ptr), &cursor_position.x, &cursor_position.y);

		return cursor_position;
	}

	void platform_win32_t::set_cursor_position(const vec2_f64_t& cursor_position) const
	{
        glfwSetCursorPos(static_cast<GLFWwindow*>(window_ptr), cursor_position.x, cursor_position.y);
	}

	bool platform_win32_t::is_cursor_hidden() const
	{
        return glfwGetInputMode(static_cast<GLFWwindow*>(window_ptr), GLFW_CURSOR) == GLFW_CURSOR_HIDDEN;
	}

	void platform_win32_t::set_cursor_hidden(bool hide_position) const
	{
        glfwSetInputMode(static_cast<GLFWwindow*>(window_ptr), GLFW_CURSOR, hide_position ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
	}

	std::string platform_win32_t::get_window_title() const
	{
		return std::string();
	}

	void platform_win32_t::set_window_title(const std::string& window_title) const
	{
        glfwSetWindowTitle(static_cast<GLFWwindow*>(window_ptr), window_title.c_str());
	}

	vec2_i32_t platform_win32_t::get_window_size() const
	{
		vec2_i32_t window_size;

        glfwGetWindowSize(static_cast<GLFWwindow*>(window_ptr), &window_size.x, &window_size.y);

		return window_size;
	}

	void platform_win32_t::set_window_size(const vec2_i32_t& window_size) const
	{
        glfwSetWindowSize(static_cast<GLFWwindow*>(window_ptr), window_size.x, window_size.y);
	}

	vec2_i32_t platform_win32_t::get_window_position() const
	{
		vec2_i32_t window_position;

        glfwGetWindowPos(static_cast<GLFWwindow*>(window_ptr), &window_position.x, &window_position.y);

		return window_position;
	}

	void platform_win32_t::set_window_position(const vec2_i32_t& window_position) const
	{
        glfwSetWindowPos(static_cast<GLFWwindow*>(window_ptr), window_position.x, window_position.y);
	}
}
