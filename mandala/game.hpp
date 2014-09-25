#pragma once

//std
#include <string>

namespace mandala
{
	struct game_t
	{
		game_t(const std::string& name);

		virtual void app_run_start();
		virtual void app_run_end();

		virtual void app_tick_start();
		virtual void app_tick_end();

		virtual void app_render_start();
		virtual void app_render_end();

		std::string name;
	};
}
