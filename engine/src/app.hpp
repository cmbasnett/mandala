#pragma once

//std
#include <chrono>
#include <array>
#include <deque>

//boost
#include <boost\shared_ptr.hpp>
#include <boost\weak_ptr.hpp>

//naga
#include "platform.hpp"
#include "api.hpp"

namespace naga
{
    struct Game;
    struct InputEvent;

    struct App
    {
        struct Performance
        {
            static const auto FRAME_COUNT = 256;

            struct Frame
            {
                typedef std::chrono::milliseconds DurationType;

				DurationType duration;
				DurationType render_duration;
				DurationType tick_duration;
				DurationType input_duration;
                size_t consumed_input_event_count = 0;
                size_t unconsumed_input_event_count = 0;
            };

            std::deque<Frame> frames;
			f32 fps = 0.0f;
        };

        const Performance& get_performance() const { return performance; }

        App() = default;

        void run(const boost::shared_ptr<Game>& game);
        void exit();
        void reset();
        void screenshot();

		f32 get_uptime_seconds() const;
        i64 get_uptime_milliseconds() const;

    private:
        bool is_exiting = false;
        bool is_resetting = false;
        Performance performance;
        std::chrono::system_clock::time_point run_time_point;
        boost::shared_ptr<Game> game;

		App(const App&) = delete;
		App& operator=(const App&) = delete;

        void tick(f32 dt);
        void render();
        void handle_input_events();
#if defined(NAGA_PC)
        void handle_window_events();
#endif
        bool should_keep_running();
    };

    extern App app;
}