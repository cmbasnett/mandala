#pragma once

//mandala
#include "mandala.h"
#include "state_mgr.h"
#include "resource_mgr.h"
#include "audio_mgr.h"

//std
#include <chrono>

namespace mandala
{
	struct game_t;
	struct game_properties_t;

	struct app_t
	{
        struct performance_t
		{
			typedef float32_t fps_type;

			struct frame_t
			{
				typedef std::chrono::milliseconds duration_type;

                duration_type duration;
                duration_type render_duration;
                duration_type tick_duration;
                duration_type input_duration;
				size_t consumed_input_event_count;
				size_t unconsumed_input_event_count;
			};

			static const size_t frames_max = 256;

			std::deque<frame_t> frames;

			performance_t()
			{
				frames.resize(frames_max);
			}
        };
		
		std::shared_ptr<game_t> game;
		state_mgr_t states;
		resource_mgr_t resources;
		performance_t performance;
		audio_mgr_t audio;

		app_t();

		void run(std::shared_ptr<game_t> game);
		void exit();
		void reset();

	private:

		bool is_exiting = false;
		bool is_resetting = false;

		app_t(const app_t&) = delete;
		app_t& operator=(const app_t&) = delete;

		void tick(float32_t dt);
		void render();
		void handle_input();
        void on_input_event(input_event_t& input_event);
		bool should_keep_running();
	};

	extern app_t app;
};
