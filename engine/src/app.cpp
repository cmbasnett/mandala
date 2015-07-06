//std
#include <chrono>

//mandala
#include "app.hpp"
#include "platform.hpp"
#include "game.hpp"

//HACK: this doesn't seem like the right place to put this
#include "model_gpu_program.hpp"
#include "bitmap_font_gpu_program.hpp"
#include "bsp_gpu_program.hpp"
#include "blur_horizontal_gpu_program.hpp"
#include "basic_gpu_program.hpp"
#include "gui_image_gpu_program.hpp"

#include "gpu_program_mgr.hpp"
#include "resource_mgr.hpp"
#include "string_mgr.hpp"
#include "audio_mgr.hpp"
#include "state_mgr.hpp"
#include "gpu_buffer_mgr.hpp"
#include "python.hpp"

namespace mandala
{
    app_t app;

	void app_t::run(const std::string& game_class)
	{
        using namespace std::chrono;
        using namespace boost;
        using namespace boost::python;

        run_time_point = std::chrono::system_clock::now();

        py.exec_file("app.py");

        this->game = extract<shared_ptr<game_t>>(py.eval((game_class + "()").c_str()))();

		begin:

        platform.app_run_start();

        gpu_programs.make<gui_image_gpu_program_t>();
        gpu_programs.make<model_gpu_program_t>();
        gpu_programs.make<bitmap_font_gpu_program_t>();
        gpu_programs.make<bsp_gpu_program_t>();
        gpu_programs.make<blur_horizontal_gpu_program_t>();
        gpu_programs.make<basic_gpu_program_t>();

        this->game->on_run_start();

        auto simulation_time = high_resolution_clock::now();

        while (should_keep_running())
        {
            auto real_time = high_resolution_clock::now();

            handle_input_events();
#if defined(MANDALA_PC)
            handle_window_events();
#endif

            while (simulation_time < real_time)
            {
                static const auto SIMULATION_INTERVAL_DURATION = milliseconds(16);
                static const auto DT = static_cast<float32_t>(SIMULATION_INTERVAL_DURATION.count()) / std::milli::den;

                simulation_time += SIMULATION_INTERVAL_DURATION;

                tick(DT);
            }

			render();

            auto b = high_resolution_clock::now() - real_time;
            auto c = b.count();

            performance.fps = 1.0f / (static_cast<float32_t>(c) / high_resolution_clock::duration::period::den);
		}

        this->game->on_run_end();
        this->game.reset();

        py.finalize();
		states.purge();
        states.tick(0); //TODO: hack to avoid exceptions throwing on close due to unreleased state objects, find a better solution later
        resources.purge();
		strings.purge();
		gpu_programs.purge();
        gpu_buffers.purge();

		platform.app_run_end();

		if (is_resetting)
		{
			is_resetting = false;

			goto begin;
		}
    }

    void app_t::exit()
    {
        is_exiting = true;
    }

	void app_t::reset()
	{
		is_resetting = true;
	}

    long long app_t::get_uptime() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - run_time_point).count();
    }

	void app_t::tick(float32_t dt)
    {
		platform.app_tick_start(dt);

        game->on_tick_start(dt);

		states.tick(dt);
		audio.tick(dt);

        game->on_tick_end(dt);

		platform.app_tick_end(dt);
	}

	void app_t::render()
	{
        const auto screen_size = platform.get_screen_size();

        gpu.viewports.push(gpu_viewport_type(0, 0, screen_size.x, screen_size.y));
        gpu.clear(gpu_t::CLEAR_FLAG_COLOR | gpu_t::CLEAR_FLAG_DEPTH);

		platform.app_render_start();

        game->on_render_start();

		states.render();

        game->on_render_end();

        platform.app_render_end();

        gpu.viewports.pop();
    }

    void app_t::handle_input_events()
    {
        input_event_t input_event;

        while (platform.pop_input_event(input_event))
        {
#if defined(MANDALA_PC)
            if (input_event.device_type == input_event_t::device_type_e::KEYBOARD &&
                input_event.keyboard.key == input_event_t::keyboard_t::key_e::F11 &&
                input_event.keyboard.type == input_event_t::keyboard_t::type_e::KEY_PRESS)
            {
                platform.set_is_fullscreen(!platform.is_fullscreen());

                continue;
            }
#endif
            //TODO: a bit ugly; if we add more things that evaluate the result
            //of on_input_event, we'll have a nested nightmare. is there a
            //better way to write this?
            //
            //perhaps implement an input_handler interface.
            if (!game->on_input_event(input_event))
            {
                states.on_input_event(input_event);
            }
        }
    }

#if defined(MANDALA_PC)
    void app_t::handle_window_events()
    {
        window_event_t window_event;

        while (platform.pop_window_event(window_event))
        {
            game->on_window_event(window_event);

            states.on_window_event(window_event);
        }
    }
#endif

	bool app_t::should_keep_running()
	{
		return !is_exiting && !is_resetting && !platform.should_exit();
	}
}
