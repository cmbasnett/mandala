#pragma once

//mandala
#include "../platform.hpp"
#include "../resource_mgr.hpp"
#include "../string_mgr.hpp"
#include "../state_mgr.hpp"
#include "../sprite_set.hpp"

//armada
#include "armada.hpp"
#include "bsp_state.hpp"
#include "debug_state.hpp"

namespace mandala
{
	namespace armada
	{
        std::shared_ptr<armada_game_t> game = std::make_shared<armada_game_t>();

        armada_game_t::armada_game_t() :
			game_t("armada")
		{
		}

        void armada_game_t::app_run_start()
        {
            game_t::app_run_start();

            platform.set_window_title(get_name());
            platform.set_window_size(vec2_i32_t(1024, 768));

            resources.mount("resources\\armada.pack");

			strings.mount("armada.strings");
			strings.language = "es";

            states.push(std::make_shared<bsp_state_t>(), state_flag_all);
            states.push(std::make_shared<debug_state_t>(), state_flag_all);
        }

        void armada_game_t::app_run_end()
		{
		}

        void armada_game_t::app_tick_start()
		{
		}

        void armada_game_t::app_tick_end()
		{
		}

        void armada_game_t::app_render_start()
		{
		}

        void armada_game_t::app_render_end()
		{
		}
	}
};
