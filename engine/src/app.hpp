#pragma once

//std
#include <chrono>
#include <array>
#include <deque>

//boost
#include <boost\shared_ptr.hpp>
#include <boost\weak_ptr.hpp>

//mandala
#include "platform.hpp"
#include "api.hpp"

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

        const performance_t& get_performance() const { return performance; }

        app_t() = default;

        void run(const std::string& game_class);
        void exit();
        void reset();
        void screenshot();

        long long get_uptime() const;

    private:
        bool is_exiting = false;
        bool is_resetting = false;
        performance_t performance;
        std::chrono::system_clock::time_point run_time_point;
        boost::shared_ptr<game_t> game;

        app_t(const app_t&) = delete;
        app_t& operator=(const app_t&) = delete;

        void tick(float32_t dt);
        void render();
        void handle_input_events();
#if defined(MANDALA_PC)
        void handle_window_events();
#endif
        bool should_keep_running();
    };

    extern app_t app;
}