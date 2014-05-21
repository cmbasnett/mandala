#pragma once

//mandala
#include "../app.h"
#include "../sprite_set.h"
#include "../platform.h"

//armada
#include "armada.h"
#include "world_state.h"
#include "pause_state.h"
#include "bsp_state.h"

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

            app.resources.mount(".\\resources\\armada.pack");

			app.states.push(std::make_shared<bsp_state_t>());
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