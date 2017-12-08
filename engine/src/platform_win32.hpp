#pragma once

#include "platform_defs.hpp"

#if defined(NAGA_WINDOWS)

//std
#include <map>

//naga
#include "platform.hpp"
#include "monitor.hpp"

struct GLFWwindow;
struct GLFWmonitor;

namespace naga
{
    struct Platform : PlatformBase
    {
		Platform() = default;

        //run
        virtual void app_run_start() override;
        virtual void app_run_end() override;

        //tick
        virtual void app_tick_start(f32 dt) override;
        virtual void app_tick_end(f32 dt) override;

        //render
        virtual void app_render_start() override;
        virtual void app_render_end() override;

        //should_exit
        virtual bool should_exit() const override;
        
        //screen_size
		virtual vec2 get_screen_size() const override;
		virtual void set_screen_size(const vec2& screen_size) const override;

        //fullscreen
        virtual bool is_fullscreen() const override;
        virtual void set_is_fullscreen(bool is_fullscreen) override;

        //pop_input_event
		virtual bool pop_input_event(InputEvent& input_event) override;

        virtual bool pop_window_event(WindowEvent& window_event) override;

        //cursor_location
		virtual vec2 get_cursor_location() const override;
		virtual void set_cursor_location(const vec2& cursor_location) const override;

        //is_cursor_hidden
        virtual bool is_cursor_hidden() const override;
        virtual void set_cursor_hidden(bool is_hidden) const override;

        //window_title
        virtual std::string get_window_title() const override;
		virtual void set_window_title(const std::string& window_title) const override;

        //window_size
        virtual vec2 get_window_size() const override;
		virtual void set_window_size(const vec2& window_size) const override;

        //window_location
		virtual vec2 get_window_location() const override;
		virtual void set_window_location(const vec2& window_location) const override;

        //clipboard
        virtual std::string get_clipboard_string() const override;
        virtual void set_clipboard_string(const std::string& clipboard) const override;

        std::map<GLFWmonitor*, size_t> monitors;

    private:
        GLFWwindow* window_ptr = nullptr;
		vec2 old_window_size;

		Platform(const Platform&) = delete;
		Platform& operator=(const Platform&) = delete;
    };

	extern Platform platform;
}

#endif