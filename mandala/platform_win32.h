#pragma once

//std
#include <array>
#include <deque>
#include <vector>

//mandala
#include "platform.h"
#include "input_event.h"

namespace mandala
{
    struct platform_win32_t : platform_t
    {
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
		virtual vec2_i32_t get_screen_size() const override;
		virtual void set_screen_size(const vec2_i32_t& screen_size) const override;

		//pop_input_event
		virtual bool pop_input_event(input_event_t& input_event) override;

		//cursor_position
		virtual vec2_f64_t get_cursor_position() const override;
		virtual void set_cursor_position(const vec2_f64_t& cursor_position) const override;

		//is_cursor_hidden
		virtual bool is_cursor_hidden() const override;
		virtual void set_cursor_hidden(bool hide_cursor) const override;

		//window_title
		virtual std::string get_window_title() const override;
		virtual void set_window_title(const std::string& window_title) const override;

		//window_size
		virtual vec2_i32_t get_window_size() const override;
		virtual void set_window_size(const vec2_i32_t& window_size) const override;

		//window_position
		virtual vec2_i32_t get_window_position() const override;
		virtual void set_window_position(const vec2_i32_t& window_position) const override;

	private:
		struct input_mgr_t
		{
			struct gamepad_state_t
			{
				std::array<float32_t, 16> axes;
				std::array<int32_t, 16> buttons;
			};

			std::deque<input_event_t> events;
			input_event_t::id_type_t event_id = 0;
			input_event_t::touch_t::touch_id_type_t touch_id = 0;
			std::array<gamepad_state_t, 4> gamepad_states;
		};

        static void on_error(int error_code, const char* message);
		static void on_keyboard_key(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void on_keyboard_character(GLFWwindow* window, unsigned int character);
        static void on_mouse_button(GLFWwindow* window, int button, int action, int mods);
        static void on_mouse_move(GLFWwindow* window, double x, double y);
        static void on_mouse_scroll(GLFWwindow* window, double x, double y);
		static void on_window_resize(GLFWwindow* window, int width, int height);

		GLFWwindow* window;
		vec4_t viewport;
		input_mgr_t input;

		platform_win32_t(const platform_win32_t&) = delete;
		platform_win32_t& operator=(const platform_win32_t&) = delete;
    };

    extern platform_win32_t platform;
};
