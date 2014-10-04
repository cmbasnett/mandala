//std
#include <chrono>

//mandala
#include "app.hpp"
#include "platform.hpp"
#include "game.hpp"

//HACK:
#include "gui_gpu_program.hpp"
#include "model_gpu_program.hpp"
#include "bitmap_font_gpu_program.hpp"
#include "bsp_gpu_program.hpp"
#include "blur_horizontal_gpu_program.hpp"
#include "basic_gpu_program.hpp"

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
            auto dt = static_cast<float32_t>(duration_cast<milliseconds>(high_resolution_clock::now() - frame_start_time).count()) / std::milli::den;

            performance.fps = 1.0f / dt;

            frame_start_time = high_resolution_clock::now();

            handle_input();
            tick(dt);
            render();
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

	void app_t::handle_input()
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

	bool app_t::should_keep_running()
	{
		return !is_exiting && !is_resetting && !platform.should_exit();
	}
}
