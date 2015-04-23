#pragma once

//boost
#include <boost\shared_ptr.hpp>

//mandala
#include "../game.hpp"

namespace mandala
{
	namespace armada
	{
		struct armada_game_t : game_t
		{
            armada_game_t();

			virtual void app_run_start();
			virtual void app_run_end();

			virtual void app_tick_start();
			virtual void app_tick_end();

			virtual void app_render_start();
			virtual void app_render_end();
		};

        extern boost::shared_ptr<armada_game_t> game;
	};
};
