#pragma once

#include "mandala.h"

namespace mandala
{
    struct input_event_t;

	struct platform_t
	{
#if defined(_WIN32) || defined(WIN32)
		bool is_cursor_centered = false;
#endif

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

#if defined(_WIN32) || defined(WIN32)
		//cursor_position
		virtual vec2_f64_t get_cursor_position() const = 0;
		virtual void set_cursor_position(const vec2_f64_t& cursor_position) const = 0;

		//is_cursor_hidden
		virtual bool is_cursor_hidden() const = 0;
		virtual void set_cursor_hidden(bool hide_cursor) const = 0;

		virtual std::string get_window_title() const = 0;
		virtual void set_window_title(const std::string& window_title) const = 0;

		virtual vec2_i32_t get_window_size() const = 0;
		virtual void set_window_size(const vec2_i32_t& window_size) const = 0;

		virtual vec2_i32_t get_window_position() const = 0;
		virtual void set_window_position(const vec2_i32_t& window_position) const = 0;
#endif

	protected:
#if defined(_WIN32) || defined(WIN32)
		vec2_f64_t cursor_position;
#endif
	};
};

//WIN32
#if defined(_WIN32) || defined(WIN32)
#include "platform_win32.h"
#endif