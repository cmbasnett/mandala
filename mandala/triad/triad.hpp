#pragma once

//std
#include <memory>

//mandala
#include "../game.hpp"

using namespace mandala;

namespace triad
{
    struct triad_game_t : game_t
    {
        triad_game_t();
        virtual ~triad_game_t();

        virtual void app_run_start();
        virtual void app_run_end();

        virtual void app_tick_start();
        virtual void app_tick_end();

        virtual void app_render_start();
        virtual void app_render_end();
    };

    extern std::shared_ptr<triad_game_t> game;
};
