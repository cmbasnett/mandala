#pragma once

//std
#include <chrono>
#include <array>
#include <deque>

//boost
#include <boost\shared_ptr.hpp>

//mandala
#include "platform.hpp"

namespace mandala
{
    struct game_t;
	struct input_event_t;

    struct app_t
    {
        struct performance_t
        {
            typedef float32_t fps_type;

            static const auto FRAME_COUNT = 256;

            struct frame_t
            {
                typedef std::chrono::milliseconds duration_type;

                duration_type duration;
                duration_type render_duration;
                duration_type tick_duration;
                duration_type input_duration;
                size_t consumed_input_event_count = 0;
                size_t unconsumed_input_event_count = 0;
            };

            std::deque<frame_t> frames;
            fps_type fps = fps_type(0);
        };

        const boost::shared_ptr<game_t> get_game() const { return game; }
        const performance_t& get_performance() const { return performance; }
        std::chrono::system_clock::duration get_frame_duration() const { return frame_duration; }

        app_t();

        void run(boost::shared_ptr<game_t> game);
        void exit();
        void reset();

        std::chrono::system_clock::duration get_uptime() const;

    private:
        bool is_exiting = false;
        bool is_resetting = false;
        boost::shared_ptr<game_t> game;
        performance_t performance;
        std::chrono::system_clock::time_point run_time_point;
        std::chrono::milliseconds frame_duration;

        app_t(const app_t&) = delete;
        app_t& operator=(const app_t&) = delete;

        void tick(float32_t dt);
        void render();
        void handle_input_events();
        void on_input_event(input_event_t& input_event);
#if defined(MANDALA_PC)
        void handle_window_events();
        void on_window_event(window_event_t& window_event);
#endif
        bool should_keep_running();
    };

    extern app_t app;
}
