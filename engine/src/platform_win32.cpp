//std
#include <array>
#include <chrono>
#include <iostream>

//naga
#include "platform_win32.hpp"

//glew
#include <GL\glew.h>

//glfw
#include <GLFW\glfw3.h>

namespace naga
{
    platform_win32 platform;

    static inline void on_keyboard_key(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        input_event_t input_event;
        input_event.device_type = input_event_t::device_type_e::KEYBOARD;
        input_event.keyboard.key = static_cast<input_event_t::keyboard_t::key_e>(key);

        if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
        {
            input_event.keyboard.mod_flags |= input_event_t::MOD_FLAG_SHIFT;
        }

        if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
        {
            input_event.keyboard.mod_flags |= input_event_t::MOD_FLAG_CTRL;
        }

        if ((mods & GLFW_MOD_ALT) == GLFW_MOD_ALT)
        {
            input_event.keyboard.mod_flags |= input_event_t::MOD_FLAG_ALT;
        }

        if ((mods & GLFW_MOD_SUPER) == GLFW_MOD_SUPER)
        {
            input_event.keyboard.mod_flags |= input_event_t::MOD_FLAG_SUPER;
        }

        switch (action)
        {
        case GLFW_RELEASE:
            input_event.keyboard.type = input_event_t::keyboard_t::type_e::KEY_RELEASE;
            break;
        case GLFW_PRESS:
            input_event.keyboard.type = input_event_t::keyboard_t::type_e::KEY_PRESS;
            break;
        case GLFW_REPEAT:
            input_event.keyboard.type = input_event_t::keyboard_t::type_e::KEY_REPEAT;
            break;
        }

        platform.input.events.push_back(input_event);
    }

    static inline void on_keyboard_character(GLFWwindow* window, unsigned int character)
    {
        input_event_t input_event;
        input_event.device_type = input_event_t::device_type_e::KEYBOARD;
        input_event.keyboard.type = input_event_t::keyboard_t::type_e::CHARACTER;
        input_event.keyboard.character = character;

        platform.input.events.push_back(input_event);
    }

    static inline void on_mouse_button(GLFWwindow* window, int button, int action, int mods)
    {
        input_event_t input_event;
        input_event.device_type = input_event_t::device_type_e::TOUCH;
        input_event.touch.button = static_cast<input_event_t::touch_t::button_e>(button);
        input_event.touch.mod_flags = mods;

        bool is_press = (action == GLFW_PRESS);

        input_event.touch.type = is_press ? input_event_t::touch_t::type_e::PRESS : input_event_t::touch_t::type_e::RELEASE;

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
        input_event.device_type = input_event_t::device_type_e::TOUCH;
        input_event.touch.type = input_event_t::touch_t::type_e::MOVE;
        input_event.touch.location.x = x;
        input_event.touch.location.y = y;
        input_event.touch.location_delta.x = x - platform.cursor_location.x;
        input_event.touch.location_delta.y = -(y - platform.cursor_location.y);

        input_event.touch.id = platform.input.touch_id;

        if (platform.is_cursor_centered)
        {
            const auto screen_size = platform.get_screen_size();

            auto cursor_location = glm::floor(static_cast<vec2_f64>(screen_size) / 2.0);

            platform.set_cursor_location(cursor_location);
        }

        platform.cursor_location = platform.get_cursor_location();

        platform.input.events.push_back(input_event);
    }

    static inline void on_mouse_scroll(GLFWwindow* window, double x, double y)
    {
        const auto screen_size = platform.get_screen_size();

        input_event_t input_event;
        input_event.device_type = input_event_t::device_type_e::TOUCH;
        input_event.touch.type = input_event_t::touch_t::type_e::SCROLL;
        input_event.touch.location = platform.get_cursor_location();
        input_event.touch.location_delta.x = x;
        input_event.touch.location_delta.y = y;

        platform.input.events.push_back(input_event);
    }

    static inline void on_window_resize(GLFWwindow* window, int width, int height)
    {
        if (window == nullptr || width == 0 || height == 0)
        {
            return;
        }

        //TODO: a less verbose solution is possible
        auto window_events_itr = std::find_if(platform.window.events.begin(), platform.window.events.end(), [](const window_event& window_event)
        {
            return window_event.type == window_event_type::RESIZE;
        });

        if (window_events_itr != platform.window.events.end())
        {
            window_events_itr->rectangle.width = width;
            window_events_itr->rectangle.height = height;
        }
        else
        {
            window_event window_event;
            window_event.type = window_event_type::RESIZE;
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
        auto window_events_itr = std::find_if(platform.window.events.begin(), platform.window.events.end(), [](const window_event& window_event)
        {
            return window_event.type == window_event_type::MOVE;
        });

        if (window_events_itr != platform.window.events.end())
        {
            window_events_itr->rectangle.x = x;
            window_events_itr->rectangle.y = y;
        }
        else
        {
            window_event window_event;
            window_event.type = window_event_type::MOVE;
            window_event.rectangle.x = x;
            window_event.rectangle.y = y;

            platform.window.events.push_back(window_event);
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

            display = std::make_unique<platform_t::display_t>();
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

    void platform_win32::app_run_start()
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

    void platform_win32::app_run_end()
    {
        glfwDestroyWindow(window_ptr);
        glfwTerminate();

        window_ptr = nullptr;
    }

    void platform_win32::app_tick_start(f32 dt)
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
                    input_event.device_type = input_event_t::device_type_e::GAMEPAD;
                    input_event.gamepad.index = gamepad_index;
                    input_event.gamepad.type = input_event_t::gamepad_t::type_e::AXIS_MOVE;
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
                    input_event.device_type = input_event_t::device_type_e::GAMEPAD;
                    input_event.gamepad.index = gamepad_index;
                    input_event.gamepad.type = buttons[button_index] == 0 ? input_event_t::gamepad_t::type_e::RELEASE : input_event_t::gamepad_t::type_e::PRESS;
                    input_event.gamepad.button_index = button_index;

                    platform.input.events.push_back(input_event);
                }

                gamepad_state.buttons[button_index] = buttons[button_index];
            }
        }
    }

    void platform_win32::app_tick_end(f32 dt)
    {
    }

    void platform_win32::app_render_start()
    {
    }

    void platform_win32::app_render_end()
    {
        glfwSwapBuffers(window_ptr);
    }

    bool platform_win32::should_exit() const
    {
        return glfwWindowShouldClose(window_ptr) != 0;
    }

    platform_t::screen_size_type platform_win32::get_screen_size() const
    {
        screen_size_type screen_size;

        glfwGetWindowSize(window_ptr, &screen_size.x, &screen_size.y);

        return screen_size;
    }
    
    void platform_win32::set_screen_size(const screen_size_type& screen_size) const
    {
        glfwSetWindowSize(window_ptr, screen_size.x, screen_size.y);
    }

    //fullscreen
    bool platform_win32::is_fullscreen() const
    {
        return window_ptr != nullptr && glfwGetWindowMonitor(window_ptr) != nullptr;
    }

    void platform_win32::set_is_fullscreen(bool is_fullscreen)
    {
        if (window_ptr == nullptr)
        {
            throw std::exception();
        }

        window_size_type window_size;

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

            window_size.x = video_mode->width;
            window_size.y = video_mode->height;
        }
        else
        {
            window_size = old_window_size;
        }

        auto new_window_ptr = glfwCreateWindow(window_size.x, window_size.y, "naga", is_fullscreen ? glfwGetPrimaryMonitor() : nullptr, window_ptr);
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
        on_window_resize(window_ptr, window_size.x, window_size.y);
    }

    bool platform_win32::pop_input_event(input_event_t& input_event)
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

    bool platform_win32::pop_window_event(window_event& window_event)
    {
        if (window.events.empty())
        {
            return false;
        }

        window_event = window.events.front();
        
        window.events.pop_front();

        return true;
    }

    platform_t::cursor_location_type platform_win32::get_cursor_location() const
    {
        cursor_location_type cursor_location;

        glfwGetCursorPos(window_ptr, &cursor_location.x, &cursor_location.y);

        return cursor_location;
    }

    void platform_win32::set_cursor_location(const cursor_location_type& cursor_location) const
    {
        glfwSetCursorPos(window_ptr, cursor_location.x, cursor_location.y);
    }

    bool platform_win32::is_cursor_hidden() const
    {
        return glfwGetInputMode(window_ptr, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN;
    }

    void platform_win32::set_cursor_hidden(bool is_hidden) const
    {
        glfwSetInputMode(window_ptr, GLFW_CURSOR, is_hidden ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
    }

    platform_t::window_title_type platform_win32::get_window_title() const
    {
        //TODO: implement this
        return std::string();
    }

    void platform_win32::set_window_title(const window_title_type& window_title) const
    {
        glfwSetWindowTitle(window_ptr, window_title.c_str());
    }

    platform_t::window_size_type platform_win32::get_window_size() const
    {
        window_size_type window_size;

        glfwGetWindowSize(window_ptr, &window_size.x, &window_size.y);

        return window_size;
    }

    void platform_win32::set_window_size(const window_size_type& window_size) const
    {
        glfwSetWindowSize(window_ptr, window_size.x, window_size.y);
    }

    platform_t::window_size_type platform_win32::get_window_location() const
    {
        vec2_i32 window_location;

        glfwGetWindowPos(window_ptr, &window_location.x, &window_location.y);

        return window_location;
    }

    void platform_win32::set_window_location(const window_size_type& window_location) const
    {
        glfwSetWindowPos(window_ptr, window_location.x, window_location.y);
    }

    std::string platform_win32::get_clipboard_string() const
    {
        return glfwGetClipboardString(window_ptr);
    }

    void platform_win32::set_clipboard_string(const std::string& clipboard_string) const
    {
        glfwSetClipboardString(window_ptr, clipboard_string.c_str());
    }
}
