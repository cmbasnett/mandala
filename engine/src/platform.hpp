#pragma once

//std
#include <deque>

//mandala
#include "types.hpp"
#include "platform_defs.hpp"
#include "input_event.hpp"

#if defined (MANDALA_PC)
#include "window_event.hpp"
#endif

namespace mandala
{
	struct platform_t
	{
        struct input_mgr_t
        {
#if defined(MANDALA_PC)
            struct gamepad_state_t
            {
                std::array<float32_t, 16> axes;
                std::array<int32_t, 16> buttons;
            };

            std::array<gamepad_state_t, 4> gamepad_states;
#endif

            std::deque<input_event_t> events;
            input_event_t::id_type event_id = 0;
            input_event_t::touch_t::touch_id_type touch_id = 1;
        } input;

#if defined(MANDALA_PC)
        struct window_mgr_t
        {
            std::deque<window_event_t> events;
            rectangle_i32_t rectangle;
        } window;
#endif

        typedef vec2_i32_t screen_size_type;

		//run
        virtual void app_run_start() = 0;
        virtual void app_run_end() = 0;

		//tick
        virtual void app_tick_start(float32_t dt) = 0;
        virtual void app_tick_end(float32_t dt) = 0;

		//render
        virtual void app_render_start() = 0;
        virtual void app_render_end() = 0;

		//should_exit
        virtual bool should_exit() const = 0;

		//screen_size
        virtual screen_size_type get_screen_size() const = 0;
		virtual void set_screen_size(const screen_size_type& screen_size) const = 0;

		//pop_input_event
        virtual bool pop_input_event(input_event_t& input_event) = 0;

		//clipboard
		virtual std::string get_clipboard_string() const = 0;
		virtual void set_clipboard_string(const std::string& clipboard) const = 0;

#if defined(MANDALA_PC)
		typedef vec2_f64_t cursor_location_type;
		typedef vec2_i32_t window_size_type;
		typedef std::string window_title_type;

		//cursor_location
		virtual cursor_location_type get_cursor_location() const = 0;
		virtual void set_cursor_location(const cursor_location_type& cursor_location) const = 0;

		//is_cursor_hidden
		virtual bool is_cursor_hidden() const = 0;
		virtual void set_cursor_hidden(bool hide_cursor) const = 0;

		virtual window_title_type get_window_title() const = 0;
		virtual void set_window_title(const window_title_type& window_title) const = 0;

		virtual window_size_type get_window_size() const = 0;
		virtual void set_window_size(const window_size_type& window_size) const = 0;

		virtual window_size_type get_window_location() const = 0;
		virtual void set_window_location(const window_size_type& window_location) const = 0;

        virtual bool is_fullscreen() const = 0;
        virtual void set_is_fullscreen(bool is_fullscreen) = 0;

        //pop_window_event
        virtual bool pop_window_event(window_event_t& window_event) = 0;

        bool is_cursor_centered = false;
        cursor_location_type cursor_location;
#endif
	};
}

#if defined(MANDALA_WINDOWS)
#include "platform_win32.hpp"
#elif defined(MANDALA_OSX)
#include "platform_osx.hpp"
#endif