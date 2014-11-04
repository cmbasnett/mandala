#pragma once

//mandala
#include "../app.hpp"
#include "../sprite_set.hpp"
#include "../platform.hpp"

//triad
#include "triad.hpp"
#include "loading_state.hpp"

using namespace mandala;

namespace triad
{
    std::shared_ptr<triad_game_t> game = std::make_shared<triad_game_t>();

    triad_game_t::triad_game_t() :
        game_t("triad")
    {
    }

    triad_game_t::~triad_game_t()
    {
    }

    void triad_game_t::app_run_start()
    {
        game_t::app_run_start();

        platform.set_window_title(name);
        platform.set_window_size(vec2_i32_t(1024, 768));

        app.resources.mount("resources\\triad.pack");

        app.states.push(std::make_shared<loading_state_t>(), state_flag_all);
    }

    void triad_game_t::app_run_end()
    {
    }

    void triad_game_t::app_tick_start()
    {
    }

    void triad_game_t::app_tick_end()
    {
    }

    void triad_game_t::app_render_start()
    {
    }

    void triad_game_t::app_render_end()
    {
    }
}
