#pragma once

//mandala
#include "platform.hpp"

struct GLFWwindow;

namespace mandala
{
    struct platform_win32_t : platform_t
    {
        platform_win32_t() = default;

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

        //fullscreen
        virtual bool is_fullscreen() const override;
        virtual void set_is_fullscreen(bool is_fullscreen) override;

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

    private:
        GLFWwindow* window_ptr = nullptr;
        window_size_type old_window_size;

		platform_win32_t(const platform_win32_t&) = delete;
		platform_win32_t& operator=(const platform_win32_t&) = delete;
    };

    extern platform_win32_t platform;
}
