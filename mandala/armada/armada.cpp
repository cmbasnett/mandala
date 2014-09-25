#pragma once

//mandala
#include "../app.hpp"
#include "../sprite_set.hpp"
#include "../platform.hpp"

//armada
#include "armada.hpp"
#include "world_state.hpp"
#include "pause_state.hpp"
#include "bsp_state.hpp"
#include "debug_state.hpp"

namespace mandala
{
	namespace armada
	{
		std::shared_ptr<armada_t> armada = std::make_shared<armada_t>();

		armada_t::armada_t() :
			game_t("armada")
		{
		}

		armada_t::~armada_t()
		{
		}

		void armada_t::app_run_start()
		{
			game_t::app_run_start();

            platform.set_window_title(name);
            platform.set_window_size(vec2_i32_t(1024, 768));

			app.resources.mount("resources\\armada.pack");

			app.states.push(std::make_shared<world_state_t>(), state_flag_all);
			//app.states.push(std::make_shared<debug_state_t>(), state_flag_all);
		}

		void armada_t::app_run_end()
		{
		}

		void armada_t::app_tick_start()
		{
		}

		void armada_t::app_tick_end()
		{
		}

		void armada_t::app_render_start()
		{
		}

		void armada_t::app_render_end()
		{
		}
	}
};
