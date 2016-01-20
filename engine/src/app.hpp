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
    struct game;
    struct input_event_t;

    struct app
    {
        struct performance_t
        {
            typedef f32 fps_type;

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

        app() = default;

        void run();
        void exit();
        void reset();
        void screenshot();

        long long get_uptime() const;

    private:
        bool is_exiting = false;
        bool is_resetting = false;
        performance_t performance;
        std::chrono::system_clock::time_point run_time_point;
        boost::shared_ptr<game> game;

        app(const app&) = delete;
        app& operator=(const app&) = delete;

        void tick(f32 dt);
        void render();
        void handle_input_events();
#if defined(NAGA_PC)
        void handle_window_events();
#endif
        bool should_keep_running();
    };

    extern app app_;
}