#pragma once

//strd
#include <memory>

//mandala
#include "../game.h"

namespace mandala
{
	namespace armada
	{
		struct armada_t : game_t
		{
			armada_t();
			virtual ~armada_t();

			virtual void app_run_start();
			virtual void app_run_end();

			virtual void app_tick_start();
			virtual void app_tick_end();

			virtual void app_render_start();
			virtual void app_render_end();
		};

		extern std::shared_ptr<armada_t> armada;
	};
};
