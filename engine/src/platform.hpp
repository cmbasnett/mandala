#pragma once

//std
#include <deque>
#include <array>
#include <vector>
#include <memory>

//naga
#include "types.hpp"
#include "platform_defs.hpp"
#include "input_event.hpp"

#if defined (NAGA_PC)
#include "window_event.hpp"
#endif

#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/base_units/imperial/inch.hpp>

namespace naga
{
    struct PlatformBase
    {
        struct InputManager
        {
#if defined(NAGA_PC)
            struct GamepadState
            {
                std::array<f32, 16> axes;
                std::array<i32, 16> buttons;
            };

			std::array<GamepadState, 4> gamepad_states;
#endif

			std::deque<InputEvent> events;
			size_t event_id = 0;
			size_t touch_id = 1;
        } input;

#if defined(NAGA_PC)
        struct WindowManager
        {
            std::deque<WindowEvent> events;
            details::Rectangle<i32> rectangle;
        } window;
#endif

        struct Display
        {
            struct VideoMode
            {
                i32 width = 0;
                i32 height = 0;
                i32 bit_depth = 0;
                i32 refresh_rate = 0;
            };

            std::string name;
            vec2_i32 position;
            f32 ppi = 0.0f;
			std::vector<VideoMode> video_modes;
        };

        //run
        virtual void app_run_start() = 0;
        virtual void app_run_end() = 0;

        //tick
        virtual void app_tick_start(f32 dt) = 0;
        virtual void app_tick_end(f32 dt) = 0;

        //render
        virtual void app_render_start() = 0;
        virtual void app_render_end() = 0;

        //should_exit
        virtual bool should_exit() const = 0;

        //screen_size
		virtual vec2 get_screen_size() const = 0;
		virtual void set_screen_size(const vec2& screen_size) const = 0;

        //pop_input_event
		virtual bool pop_input_event(InputEvent& input_event) = 0;

        //clipboard
        virtual std::string get_clipboard_string() const = 0;
        virtual void set_clipboard_string(const std::string& clipboard) const = 0;

#if defined(NAGA_PC)
        //cursor_location
		virtual vec2 get_cursor_location() const = 0;
		virtual void set_cursor_location(const vec2& cursor_location) const = 0;

        //is_cursor_hidden
        virtual bool is_cursor_hidden() const = 0;
        virtual void set_cursor_hidden(bool hide_cursor) const = 0;

        virtual std::string get_window_title() const = 0;
        virtual void set_window_title(const std::string& window_title) const = 0;

		virtual vec2 get_window_size() const = 0;
		virtual void set_window_size(const vec2& window_size) const = 0;

		virtual vec2 get_window_location() const = 0;
		virtual void set_window_location(const vec2& window_location) const = 0;

        virtual bool is_fullscreen() const = 0;
        virtual void set_is_fullscreen(bool is_fullscreen) = 0;

        //pop_window_event
        virtual bool pop_window_event(WindowEvent& window_event) = 0;

        bool is_cursor_centered = false;
		vec2 cursor_location;

        const Display& get_display(size_t index) const;

		std::array<std::unique_ptr<Display>, 8> displays;
#endif
    };
}

#if defined(NAGA_WINDOWS)
#include "platform_win32.hpp"
#elif defined(NAGA_OSX)
#include "platform_osx.hpp"
#endif