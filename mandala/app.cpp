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
#include "gui_gpu_program.hpp"

#include "gpu_program_mgr.hpp"
#include "resource_mgr.hpp"
#include "string_mgr.hpp"
#include "audio_mgr.hpp"
#include "http_mgr.hpp"
#include "state_mgr.hpp"

namespace mandala
{
    app_t app;

    app_t::app_t()
    {
    }

	void app_t::run(std::shared_ptr<game_t> game_)
	{
		begin:
		using namespace std::chrono;

        run_time_point = system_clock::now();

		game = game_;

		platform.app_run_start();

        gpu_programs.make<gui_gpu_program_t>();
        gpu_programs.make<model_gpu_program_t>();
        gpu_programs.make<bitmap_font_gpu_program_t>();
        gpu_programs.make<bsp_gpu_program_t>();
        gpu_programs.make<blur_horizontal_gpu_program_t>();
        gpu_programs.make<basic_gpu_program_t>();

		game->app_run_start();

		auto frame_start_time = high_resolution_clock::now();

        while (should_keep_running())
        {
            frame_duration = duration_cast<milliseconds>(high_resolution_clock::now() - frame_start_time);

            auto dt = static_cast<float32_t>(frame_duration.count()) / std::milli::den;

            performance.fps = 1.0f / dt;
            frame_start_time = high_resolution_clock::now();

            handle_input_events();
            tick(dt);
			render();

#if defined(MANDALA_PC)
			handle_window_events();
#endif
		}

		game->app_run_end();

		states.purge();
		resources.purge();
		strings.purge();
		gpu_programs.purge();

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

    std::chrono::system_clock::duration app_t::get_uptime() const
    {
        return std::chrono::system_clock::now() - run_time_point;
    }

	void app_t::tick(float32_t dt)
    {
		platform.app_tick_start(dt);

		game->app_tick_start();

		states.tick(dt);
		audio.tick(dt);

		game->app_tick_end();

		platform.app_tick_end(dt);
	}

	void app_t::render()
	{
		platform.app_render_start();

		game->app_render_start();

		states.render();

		game->app_render_end();

        platform.app_render_end();
    }

    void app_t::handle_input_events()
    {
        input_event_t input_event;

        while (platform.pop_input_event(input_event))
        {
            on_input_event(input_event);
        }
    }

    void app_t::on_input_event(input_event_t& input_event)
	{
		states.on_input_event(input_event);
    }

#if defined(MANDALA_PC)
    void app_t::handle_window_events()
    {
        window_event_t window_event;

        while (platform.pop_window_event(window_event))
        {
            on_window_event(window_event);
        }
    }

    void app_t::on_window_event(window_event_t& window_event)
    {
        states.on_window_event(window_event);
    }
#endif

	bool app_t::should_keep_running()
	{
		return !is_exiting && !is_resetting && !platform.should_exit();
	}
}
