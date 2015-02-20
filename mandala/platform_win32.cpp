//std
#include <array>
#include <chrono>
#include <iostream>

//mandala
#include "platform_win32.hpp"
#include "gpu.hpp"

//glew
#include <GL\glew.h>

//glfw
#include <GLFW\glfw3.h>


namespace mandala
{
    platform_win32_t platform;

    static inline void on_keyboard_key(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		input_event_t input_event;
		input_event.device_type = input_event_t::device_type_e::keyboard;
		input_event.keyboard.key = static_cast<input_event_t::keyboard_t::key_e>(key);

		if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
		{
			input_event.keyboard.mod_flags |= input_event_t::mod_flag_shift;
		}

        if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
        {
			input_event.keyboard.mod_flags |= input_event_t::mod_flag_ctrl;
        }

        if ((mods & GLFW_MOD_ALT) == GLFW_MOD_ALT)
        {
			input_event.keyboard.mod_flags |= input_event_t::mod_flag_alt;
        }

        if ((mods & GLFW_MOD_SUPER) == GLFW_MOD_SUPER)
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

        bool is_press = (action == GLFW_PRESS);

        input_event.touch.type = is_press ? input_event_t::touch_t::type_e::press : input_event_t::touch_t::type_e::release;

        if (is_press)
        {
            ++platform.input.touch_id;

            input_event.touch.id = platform.input.touch_id;
        }
        else
        {
            platform.input.touch_id = 0;    //TODO: replace with something better
        }

        input_event.touch.location = platform.get_cursor_location();

        platform.input.events.push_back(input_event);
    }

    static inline void on_mouse_move(GLFWwindow* window, double x, double y)
	{
        const auto screen_size = platform.get_screen_size();

		input_event_t input_event;
		input_event.device_type = input_event_t::device_type_e::touch;
		input_event.touch.type = input_event_t::touch_t::type_e::move;
		input_event.touch.location.x = x;
		input_event.touch.location.y = screen_size.y - y;
		input_event.touch.location_delta.x = x - platform.cursor_location.x;
        input_event.touch.location_delta.y = -(y - platform.cursor_location.y);

        input_event.touch.id = platform.input.touch_id;

		if (platform.is_cursor_centered)
		{
			const auto screen_size = platform.get_screen_size();

            auto cursor_location = glm::floor(static_cast<vec2_f64_t>(screen_size) / 2.0);

			platform.set_cursor_location(cursor_location);
		}

        platform.cursor_location = platform.get_cursor_location();

        platform.input.events.push_back(input_event);
    }

    static inline void on_mouse_scroll(GLFWwindow* window, double x, double y)
    {
        const auto screen_size = platform.get_screen_size();

        input_event_t input_event;
        input_event.device_type = input_event_t::device_type_e::touch;
        input_event.touch.type = input_event_t::touch_t::type_e::scroll;
        input_event.touch.location = platform.get_cursor_location();
        input_event.touch.location_delta.x = x;
        input_event.touch.location_delta.y = y;

        platform.input.events.push_back(input_event);
    }

    static inline void on_window_resize(GLFWwindow* window, int width, int height)
	{
		//TODO: a less vebose solution is possible
        auto window_events_itr = std::find_if(platform.window.events.begin(), platform.window.events.end(), [](const window_event_t& window_event)
        {
            return window_event.type == window_event_t::type_e::resize;
        });

        if (window_events_itr != platform.window.events.end())
        {
            window_events_itr->rectangle.width = width;
			window_events_itr->rectangle.height = height;
        }
        else
        {
            window_event_t window_event;
            window_event.type = window_event_t::type_e::resize;
			window_event.rectangle.width = width;
			window_event.rectangle.height = height;

            platform.window.events.push_back(window_event);
        }

        platform.window.rectangle.width = width;
        platform.window.rectangle.height = height;
    }

    static inline void on_window_move(GLFWwindow* window, int x, int y)
	{
		//TODO: a less vebose solution is possible
        auto window_events_itr = std::find_if(platform.window.events.begin(), platform.window.events.end(), [](const window_event_t& window_event)
        {
            return window_event.type == window_event_t::type_e::move;
        });

        if (window_events_itr != platform.window.events.end())
        {
			window_events_itr->rectangle.x = x;
			window_events_itr->rectangle.y = y;
        }
        else
        {
            window_event_t window_event;
            window_event.type = window_event_t::type_e::move;
			window_event.rectangle.x = x;
			window_event.rectangle.y = y;

            platform.window.events.push_back(window_event);
        }

        platform.window.rectangle.x = x;
        platform.window.rectangle.y = y;
    }

    static inline void on_error(int error_code, const char* message)
    {
        throw std::exception();
    }

	void platform_win32_t::app_run_start()
	{
		//glfw
		glfwInit();

		window_ptr = glfwCreateWindow(1, 1, "mandala", nullptr, nullptr);

        glfwMakeContextCurrent(window_ptr);

        glfwSetKeyCallback(window_ptr, on_keyboard_key);
        glfwSetCharCallback(window_ptr, on_keyboard_character);
        glfwSetMouseButtonCallback(window_ptr, on_mouse_button);
        glfwSetCursorPosCallback(window_ptr, on_mouse_move);
        glfwSetScrollCallback(window_ptr, on_mouse_scroll);
        glfwSetWindowSizeCallback(window_ptr, on_window_resize);
        glfwSetWindowPosCallback(window_ptr, on_window_move);

		//glew
        auto glew_init_result = glewInit();

        if (glew_init_result != GLEW_OK)
        {
            throw std::exception(unsafe_cast<char*>(glewGetErrorString(glew_init_result)));
        }
	}

	void platform_win32_t::app_run_end()
	{
        glfwDestroyWindow(window_ptr);
        glfwTerminate();

        window_ptr = nullptr;
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
					input_event.gamepad.index = gamepad_index;
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
					input_event.gamepad.index = gamepad_index;
					input_event.gamepad.type = buttons[button_index] == 0 ? input_event_t::gamepad_t::type_e::release : input_event_t::gamepad_t::type_e::press;
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
        gpu.clear(gpu_t::clear_flag_color | gpu_t::clear_flag_depth);
	}

	void platform_win32_t::app_render_end()
	{
        glfwSwapBuffers(window_ptr);
	}

	bool platform_win32_t::should_exit() const
	{
        return glfwWindowShouldClose(window_ptr) != 0;
	}

    platform_t::screen_size_type platform_win32_t::get_screen_size() const
    {
        screen_size_type screen_size;

        glfwGetWindowSize(window_ptr, &screen_size.x, &screen_size.y);

		return screen_size;
    }
	
	void platform_win32_t::set_screen_size(const screen_size_type& screen_size) const
	{
        glfwSetWindowSize(window_ptr, screen_size.x, screen_size.y);
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

    bool platform_win32_t::pop_window_event(window_event_t& window_event)
    {
        if (window.events.empty())
        {
            return false;
        }

        window_event = window.events.front();
        
        window.events.pop_front();

        return true;
    }

	platform_t::cursor_location_type platform_win32_t::get_cursor_location() const
	{
        cursor_location_type cursor_location;

        glfwGetCursorPos(window_ptr, &cursor_location.x, &cursor_location.y);

		return cursor_location;
	}

	void platform_win32_t::set_cursor_location(const cursor_location_type& cursor_location) const
	{
        glfwSetCursorPos(window_ptr, cursor_location.x, cursor_location.y);
	}

	bool platform_win32_t::is_cursor_hidden() const
	{
        return glfwGetInputMode(window_ptr, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN;
	}

	void platform_win32_t::set_cursor_hidden(bool is_hidden) const
	{
        glfwSetInputMode(window_ptr, GLFW_CURSOR, is_hidden ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
	}

	platform_t::window_title_type platform_win32_t::get_window_title() const
	{
		//TODO: implement this
		return std::string();
	}

	void platform_win32_t::set_window_title(const window_title_type& window_title) const
	{
        glfwSetWindowTitle(window_ptr, window_title.c_str());
	}

    platform_t::window_size_type platform_win32_t::get_window_size() const
	{
        window_size_type window_size;

        glfwGetWindowSize(window_ptr, &window_size.x, &window_size.y);

		return window_size;
	}

	void platform_win32_t::set_window_size(const window_size_type& window_size) const
	{
        glfwSetWindowSize(window_ptr, window_size.x, window_size.y);
	}

	platform_t::window_size_type platform_win32_t::get_window_location() const
	{
		vec2_i32_t window_location;

        glfwGetWindowPos(window_ptr, &window_location.x, &window_location.y);

		return window_location;
	}

	void platform_win32_t::set_window_location(const window_size_type& window_location) const
	{
        glfwSetWindowPos(window_ptr, window_location.x, window_location.y);
	}

	std::string platform_win32_t::get_clipboard_string() const
	{
		return glfwGetClipboardString(window_ptr);
	}

	void platform_win32_t::set_clipboard_string(const std::string& clipboard_string) const
	{
		glfwSetClipboardString(window_ptr, clipboard_string.c_str());
	}
}
