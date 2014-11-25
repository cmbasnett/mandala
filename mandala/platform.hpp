#pragma once

//mandala
#include "types.hpp"

namespace mandala
{
    struct input_event_t;
    struct window_event_t;

	struct platform_t
	{
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
        virtual vec2_i32_t get_screen_size() const = 0;
		virtual void set_screen_size(const vec2_i32_t& screen_size) const = 0;

		//pop_input_event
        virtual bool pop_input_event(input_event_t& input_event) = 0;

		//clipboard
		virtual std::string get_clipboard_string() const = 0;
		virtual void set_clipboard_string(const std::string& clipboard) const = 0;

#if defined(MANDALA_PC)
		typedef vec2_f64_t cursor_position_type;
		typedef vec2_i32_t window_size_type;
		typedef std::string window_title_type;

		//cursor_position
		virtual cursor_position_type get_cursor_position() const = 0;
		virtual void set_cursor_position(const cursor_position_type& cursor_position) const = 0;

		//is_cursor_hidden
		virtual bool is_cursor_hidden() const = 0;
		virtual void set_cursor_hidden(bool hide_cursor) const = 0;

		virtual window_title_type get_window_title() const = 0;
		virtual void set_window_title(const window_title_type& window_title) const = 0;

		virtual window_size_type get_window_size() const = 0;
		virtual void set_window_size(const window_size_type& window_size) const = 0;

		virtual window_size_type get_window_position() const = 0;
		virtual void set_window_position(const window_size_type& window_position) const = 0;

        //pop_window_event
        virtual bool pop_window_event(window_event_t& window_event) = 0;
#endif
	};
}

#if defined(MANDALA_WINDOWS)
#include "platform_win32.hpp"
#endif