#pragma once

//mandala
#include "state_mgr.hpp"
#include "resource_mgr.hpp"
#include "audio_mgr.hpp"
#include "string_mgr.hpp"
#include "gpu_program_mgr.hpp"
#include "lua_mgr.hpp"

//std
#include <chrono>
#include <array>

namespace mandala
{
    struct game_t;
    struct game_properties_t;

    struct app_t
    {
        struct performance_t
        {
            typedef float32_t fps_type;

            static const auto frame_count = 256;

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

        std::shared_ptr<game_t> game;
        state_mgr_t states;
        resource_mgr_t resources;
        performance_t performance;
        string_mgr_t strings;
        gpu_program_mgr_t gpu_programs;
        audio_mgr_t audio;
        lua_mgr_t lua;

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
        void handle_input_events();
        void on_input_event(input_event_t& input_event);
#if defined(_WIN32) || defined(WIN32)
        void handle_window_events();
        void on_window_event(window_event_t& window_event);
#endif
        bool should_keep_running();
    };

    extern app_t app;
}
