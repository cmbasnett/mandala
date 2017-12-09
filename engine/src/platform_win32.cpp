//naga
#include "platform_win32.hpp"

//std
#include <array>
#include <chrono>
#include <iostream>

//glew
#include <GL\glew.h>

//glfw
#include <GLFW\glfw3.h>

namespace naga
{
	Platform platform;

    static inline void on_keyboard_key(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
		InputEvent input_event;
		input_event.device_type = InputEvent::DeviceType::KEYBOARD;
		input_event.keyboard.key = static_cast<InputEvent::Keyboard::Key>(key);

        if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
        {
			input_event.keyboard.mod_flags |= InputEvent::MOD_FLAG_SHIFT;
        }

        if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
        {
			input_event.keyboard.mod_flags |= InputEvent::MOD_FLAG_CTRL;
        }

        if ((mods & GLFW_MOD_ALT) == GLFW_MOD_ALT)
        {
			input_event.keyboard.mod_flags |= InputEvent::MOD_FLAG_ALT;
        }

        if ((mods & GLFW_MOD_SUPER) == GLFW_MOD_SUPER)
        {
			input_event.keyboard.mod_flags |= InputEvent::MOD_FLAG_SUPER;
        }

        switch (action)
        {
        case GLFW_RELEASE:
			input_event.keyboard.type = InputEvent::Keyboard::Type::KEY_RELEASE;
            break;
        case GLFW_PRESS:
			input_event.keyboard.type = InputEvent::Keyboard::Type::KEY_PRESS;
            break;
        case GLFW_REPEAT:
			input_event.keyboard.type = InputEvent::Keyboard::Type::KEY_REPEAT;
            break;
        }

        platform.input.events.push_back(input_event);
    }

    static inline void on_keyboard_character(GLFWwindow* window, unsigned int character)
    {
        InputEvent input_event;
		input_event.device_type = InputEvent::DeviceType::KEYBOARD;
		input_event.keyboard.type = InputEvent::Keyboard::Type::CHARACTER;
        input_event.keyboard.character = character;

        platform.input.events.emplace_back(input_event);
    }

    static inline void on_mouse_button(GLFWwindow* window, int button, int action, int mods)
    {
		InputEvent input_event;
		input_event.device_type = InputEvent::DeviceType::TOUCH;
		input_event.touch.button = static_cast<InputEvent::Touch::Button>(button);
        input_event.touch.mod_flags = mods;

        bool is_press = (action == GLFW_PRESS);

		input_event.touch.type = is_press ? InputEvent::Touch::Type::PRESS : InputEvent::Touch::Type::RELEASE;

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

		InputEvent input_event;
		input_event.device_type = InputEvent::DeviceType::TOUCH;
		input_event.touch.type = InputEvent::Touch::Type::MOVE;
		input_event.touch.location.x = static_cast<f32>(x);
		input_event.touch.location.y = static_cast<f32>(y);
		input_event.touch.location_delta.x = static_cast<f32>(x) - platform.cursor_location.x;
		input_event.touch.location_delta.y = -(static_cast<f32>(y) - platform.cursor_location.y);

        input_event.touch.id = platform.input.touch_id;

        if (platform.is_cursor_centered)
        {
            const auto screen_size = platform.get_screen_size();

            auto cursor_location = glm::floor(static_cast<vec2>(screen_size) / 2.0f);

            platform.set_cursor_location(cursor_location);
        }

        platform.cursor_location = platform.get_cursor_location();

        platform.input.events.push_back(input_event);
    }

    static inline void on_mouse_scroll(GLFWwindow* window, double x, double y)
    {
        const auto screen_size = platform.get_screen_size();

		InputEvent input_event;
		input_event.device_type = InputEvent::DeviceType::TOUCH;
		input_event.touch.type = InputEvent::Touch::Type::SCROLL;
        input_event.touch.location = platform.get_cursor_location();
		input_event.touch.location_delta.x = static_cast<f32>(x);
		input_event.touch.location_delta.y = static_cast<f32>(y);

        platform.input.events.push_back(input_event);
    }

    static inline void on_window_resize(GLFWwindow* window, int width, int height)
    {
        if (window == nullptr || width == 0 || height == 0)
        {
            return;
        }

        //TODO: a less verbose solution is possible
        auto window_events_itr = std::find_if(platform.window.events.begin(), platform.window.events.end(), [](const WindowEvent& window_event)
        {
            return window_event.type == WindowEventType::RESIZE;
        });

        if (window_events_itr != platform.window.events.end())
        {
            window_events_itr->rectangle.width = width;
            window_events_itr->rectangle.height = height;
        }
        else
        {
			WindowEvent window_event;
            window_event.type = WindowEventType::RESIZE;
            window_event.rectangle.width = width;
            window_event.rectangle.height = height;

            platform.window.events.emplace_back(window_event);
        }

        platform.window.rectangle.width = width;
        platform.window.rectangle.height = height;
    }

    static inline void on_window_move(GLFWwindow* window, int x, int y)
    {
        //TODO: a less vebose solution is possible
		auto window_events_itr = std::find_if(platform.window.events.begin(), platform.window.events.end(), [](const WindowEvent& window_event)
        {
            return window_event.type == WindowEventType::MOVE;
        });

        if (window_events_itr != platform.window.events.end())
        {
            window_events_itr->rectangle.x = x;
            window_events_itr->rectangle.y = y;
        }
        else
        {
			WindowEvent window_event;
			window_event.type = WindowEventType::MOVE;
            window_event.rectangle.x = x;
            window_event.rectangle.y = y;

            platform.window.events.emplace_back(window_event);
        }

        platform.window.rectangle.x = x;
        platform.window.rectangle.y = y;
    }

    static inline void on_monitor_event(GLFWmonitor* monitor, int event)
    {
        using namespace boost::units;

        if (event == GLFW_CONNECTED)
        {
            auto display_itr = platform.displays.begin();

            for (; display_itr != platform.displays.end(); ++display_itr)
            {
                if (*display_itr == nullptr)
                {
                    break;
                }
            }

            if (display_itr == platform.displays.end())
            {
                return;
            }

            auto& display = (*display_itr);

            display = std::make_unique<Platform::Display>();
            display->name = glfwGetMonitorName(monitor);

            //https://en.wikipedia.org/wiki/Pixel_density#Calculation_of_monitor_PPI
            i32 width = 0;
            i32 height = 0;
            glfwGetMonitorPhysicalSize(monitor, &width, &height);
            glfwGetMonitorPos(monitor, &display->position.x, &display->position.y);

            vec2 physical_size(static_cast<f32>(width) / std::milli::den, static_cast<f32>(height) / std::milli::den);
            physical_size *= conversion_factor(si::meter_base_unit::unit_type(), imperial::inch_base_unit::unit_type());

            auto video_mode = glfwGetVideoMode(monitor);
            auto dp = glm::sqrt(static_cast<f32>((video_mode->width * video_mode->width) + (video_mode->height * video_mode->height)));
            auto di = glm::sqrt(static_cast<f32>((physical_size.x * physical_size.x) + (physical_size.y * physical_size.y)));

            display->ppi = (dp / di);

            i32 video_mode_count = 0;

            display->video_modes.reserve(video_mode_count);

            auto video_mode_ptr = glfwGetVideoModes(monitor, &video_mode_count);

            for (auto i = 0; i < video_mode_count; ++i)
            {
                auto video_mode_itr = display->video_modes.emplace(display->video_modes.end());

                video_mode_itr->width = video_mode_ptr->width;
                video_mode_itr->height = video_mode_ptr->height;
                video_mode_itr->bit_depth = video_mode_ptr->blueBits + video_mode_ptr->greenBits + video_mode_ptr->redBits;
                video_mode_itr->refresh_rate = video_mode_ptr->refreshRate;

                ++video_mode_ptr;
            }
        }
        else if (event == GLFW_DISCONNECTED)
        {
            auto monitors_itr = platform.monitors.find(monitor);

#if defined(DEBUG)
            assert(monitors_itr != platform.monitors.end());
#endif

            platform.monitors.erase(monitors_itr);

            platform.displays[monitors_itr->second] = nullptr;
        }
    }

    static inline void on_error(int error_code, const char* message)
    {
        throw std::exception(message);
    }

	void Platform::app_run_start()
    {
        //glfw
        glfwInit();

        i32 monitor_count = 0;
        auto monitor_ptrs = glfwGetMonitors(&monitor_count);

        if (monitor_ptrs != nullptr)
        {
            for (i32 i = 0; i < monitor_count; ++i)
            {
                on_monitor_event(monitor_ptrs[i], GLFW_CONNECTED);
            }
        }

		glfwSetErrorCallback(on_error);

        window_ptr = glfwCreateWindow(1, 1, "naga", nullptr, nullptr);

		if (window_ptr == nullptr)
		{
			throw std::exception("Failed to create window!");
		}

        glfwMakeContextCurrent(window_ptr);

        glfwSetKeyCallback(window_ptr, on_keyboard_key);
        glfwSetCharCallback(window_ptr, on_keyboard_character);
        glfwSetMouseButtonCallback(window_ptr, on_mouse_button);
        glfwSetCursorPosCallback(window_ptr, on_mouse_move);
        glfwSetScrollCallback(window_ptr, on_mouse_scroll);
        glfwSetWindowSizeCallback(window_ptr, on_window_resize);
        glfwSetWindowPosCallback(window_ptr, on_window_move);
        glfwSetMonitorCallback(on_monitor_event);

        //glew
        auto glew_init_result = glewInit();

        if (glew_init_result != GLEW_OK)
        {
            throw std::exception(unsafe_cast<char*>(glewGetErrorString(glew_init_result)));
        }
    }

	void Platform::app_run_end()
    {
        glfwDestroyWindow(window_ptr);
        glfwTerminate();

        window_ptr = nullptr;
    }

	void Platform::app_tick_start(f32 dt)
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
                    InputEvent input_event;
					input_event.device_type = InputEvent::DeviceType::GAMEPAD;
                    input_event.gamepad.index = gamepad_index;
					input_event.gamepad.type = InputEvent::Gamepad::Type::AXIS_MOVE;
                    input_event.gamepad.axis_index = axis_index;
                    input_event.gamepad.axis_value = axes[axis_index];
                    input_event.gamepad.axis_value_delta = axes[axis_index] - gamepad_state.axes[axis_index];

                    platform.input.events.emplace_back(input_event);
                }

                gamepad_state.axes[axis_index] = axes[axis_index];
            }

            int button_count = 0;
            auto buttons = glfwGetJoystickButtons(static_cast<int>(gamepad_index), &button_count);

            for (auto button_index = 0; button_index < button_count; ++button_index)
            {
                if (buttons[button_index] != gamepad_state.buttons[button_index])
                {
                    InputEvent input_event;
					input_event.device_type = InputEvent::DeviceType::GAMEPAD;
                    input_event.gamepad.index = gamepad_index;
					input_event.gamepad.type = buttons[button_index] == 0 ? InputEvent::Gamepad::Type::RELEASE : InputEvent::Gamepad::Type::PRESS;
                    input_event.gamepad.button_index = button_index;

                    platform.input.events.push_back(input_event);
                }

                gamepad_state.buttons[button_index] = buttons[button_index];
            }
        }
    }

	void Platform::app_tick_end(f32 dt)
    {
    }

	void Platform::app_render_start()
    {
    }

	void Platform::app_render_end()
    {
        glfwSwapBuffers(window_ptr);
    }

	bool Platform::should_exit() const
    {
        return glfwWindowShouldClose(window_ptr) != 0;
    }

	vec2 Platform::get_screen_size() const
    {
        vec2_i32 screen_size;

        glfwGetWindowSize(window_ptr, &screen_size.x, &screen_size.y);

		return static_cast<vec2>(screen_size);
    }
    
	void Platform::set_screen_size(const vec2& screen_size) const
	{
		glfwSetWindowSize(window_ptr, static_cast<int>(screen_size.x), static_cast<int>(screen_size.y));
	}

	// viewport
	Rectangle Platform::get_viewport() const
	{
		Rectangle viewport;
		viewport.width = get_screen_size().x;
		viewport.height = get_screen_size().y;
		return viewport;
	}

    //fullscreen
    bool Platform::is_fullscreen() const
    {
        return window_ptr != nullptr && glfwGetWindowMonitor(window_ptr) != nullptr;
    }

	void Platform::set_is_fullscreen(bool is_fullscreen)
    {
        if (window_ptr == nullptr)
        {
            throw std::exception();
        }

        vec2 window_size;

        if (is_fullscreen == this->is_fullscreen())
        {
            return;
        }

        if (is_fullscreen)
        {
            const auto video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

            glfwWindowHint(GLFW_RED_BITS, video_mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, video_mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, video_mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, video_mode->refreshRate);

            old_window_size = get_window_size();

			window_size.x = static_cast<f32>(video_mode->width);
			window_size.y = static_cast<f32>(video_mode->height);
        }
        else
        {
            window_size = old_window_size;
        }

        auto new_window_ptr = glfwCreateWindow(static_cast<int>(window_size.x), static_cast<int>(window_size.y), "naga", is_fullscreen ? glfwGetPrimaryMonitor() : nullptr, window_ptr);
        glfwDestroyWindow(window_ptr);

        window_ptr = new_window_ptr;

        glfwMakeContextCurrent(window_ptr);

        glfwSetKeyCallback(window_ptr, on_keyboard_key);
        glfwSetCharCallback(window_ptr, on_keyboard_character);
        glfwSetMouseButtonCallback(window_ptr, on_mouse_button);
        glfwSetCursorPosCallback(window_ptr, on_mouse_move);
        glfwSetScrollCallback(window_ptr, on_mouse_scroll);
        glfwSetWindowSizeCallback(window_ptr, on_window_resize);
        glfwSetWindowPosCallback(window_ptr, on_window_move);

        //HACK: would use set_window_size, but the callback doesn't get triggered
		on_window_resize(window_ptr, static_cast<int>(window_size.x), static_cast<int>(window_size.y));
    }

    bool Platform::pop_input_event(InputEvent& input_event)
    {
        if (input.events.empty())
        {
            return false;
        }

        input_event = input.events.front();
        input_event.id = input.event_id;

        input.events.pop_front();

        ++input.event_id;

        return true;
    }

	bool Platform::pop_window_event(WindowEvent& window_event)
    {
        if (window.events.empty())
        {
            return false;
        }

        window_event = window.events.front();
        
        window.events.pop_front();

        return true;
    }

	vec2 Platform::get_cursor_location() const
    {
        vec2_f64 cursor_location;

        glfwGetCursorPos(window_ptr, &cursor_location.x, &cursor_location.y);

		return static_cast<vec2>(cursor_location);
    }

	void Platform::set_cursor_location(const vec2& cursor_location) const
    {
        glfwSetCursorPos(window_ptr, cursor_location.x, cursor_location.y);
    }

	bool Platform::is_cursor_hidden() const
    {
        return glfwGetInputMode(window_ptr, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN;
    }

	void Platform::set_cursor_hidden(bool is_hidden) const
    {
        glfwSetInputMode(window_ptr, GLFW_CURSOR, is_hidden ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
    }

	std::string Platform::get_window_title() const
    {
        //TODO: implement this
        return std::string();
    }

	void Platform::set_window_title(const std::string& window_title) const
    {
        glfwSetWindowTitle(window_ptr, window_title.c_str());
    }

	vec2 Platform::get_window_size() const
    {
        vec2_i32 window_size;

        glfwGetWindowSize(window_ptr, &window_size.x, &window_size.y);

		return static_cast<vec2>(window_size);
    }

	void Platform::set_window_size(const vec2& window_size) const
    {
		glfwSetWindowSize(window_ptr, static_cast<int>(window_size.x), static_cast<int>(window_size.y));
    }

	vec2 Platform::get_window_location() const
    {
        vec2_i32 window_location;

        glfwGetWindowPos(window_ptr, &window_location.x, &window_location.y);

		return static_cast<vec2>(window_location);
    }

	void Platform::set_window_location(const vec2& window_location) const
    {
		glfwSetWindowPos(window_ptr, static_cast<int>(window_location.x), static_cast<int>(window_location.y));
    }

	std::string Platform::get_clipboard_string() const
    {
        return glfwGetClipboardString(window_ptr);
    }

	void Platform::set_clipboard_string(const std::string& clipboard_string) const
    {
        glfwSetClipboardString(window_ptr, clipboard_string.c_str());
    }
}