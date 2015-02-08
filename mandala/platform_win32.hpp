#pragma once

//std
#include <array>
#include <deque>
#include <vector>

//mandala
#include "platform.hpp"
#include "input_event.hpp"
#include "window_event.hpp"
#include "rectangle.hpp"

struct GLFWwindow;

namespace mandala
{
    struct platform_win32_t : platform_t
    {
        struct input_mgr_t
        {
            struct gamepad_state_t
            {
                std::array<float32_t, 16> axes;
                std::array<int32_t, 16> buttons;
            };

            std::deque<input_event_t> events;
            input_event_t::id_type event_id = 0;
            input_event_t::touch_t::touch_id_type touch_id = 1;
            std::array<gamepad_state_t, 4> gamepad_states;
        };

        struct window_mgr_t
        {
            std::deque<window_event_t> events;
            rectangle_i32_t rectangle;
        };

        platform_win32_t();
        virtual ~platform_win32_t();

        //run
		virtual void app_run_start() override;
		virtual void app_run_end() override;

        //tick
		virtual void app_tick_start(float32_t dt) override;
		virtual void app_tick_end(float32_t dt) override;

        //render
		virtual void app_render_start() override;
		virtual void app_render_end() override;

		//should_exit
		virtual bool should_exit() const override;
        
		//screen_size
		virtual screen_size_type get_screen_size() const override;
        virtual void set_screen_size(const screen_size_type& screen_size) const override;

		//pop_input_event
		virtual bool pop_input_event(input_event_t& input_event) override;

        virtual bool pop_window_event(window_event_t& window_event) override;

		//cursor_location
		virtual cursor_location_type get_cursor_location() const override;
		virtual void set_cursor_location(const cursor_location_type& cursor_location) const override;

		//is_cursor_hidden
		virtual bool is_cursor_hidden() const override;
		virtual void set_cursor_hidden(bool is_hidden) const override;

		//window_title
		virtual window_title_type get_window_title() const override;
		virtual void set_window_title(const window_title_type& window_title) const override;

		//window_size
		virtual window_size_type get_window_size() const override;
		virtual void set_window_size(const window_size_type& window_size) const override;

		//window_location
		virtual window_size_type get_window_location() const override;
		virtual void set_window_location(const window_size_type& window_location) const override;

		//clipboard
		virtual std::string get_clipboard_string() const override;
		virtual void set_clipboard_string(const std::string& clipboard) const override;

        GLFWwindow* window_ptr = nullptr;
		vec4_t viewport;
		input_mgr_t input;
        window_mgr_t window;
        bool is_cursor_centered = false;
        cursor_location_type cursor_location;

		platform_win32_t(const platform_win32_t&) = delete;
		platform_win32_t& operator=(const platform_win32_t&) = delete;
    };

    extern platform_win32_t platform;
}
