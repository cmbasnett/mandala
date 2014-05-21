//std
#include <fstream>

//mandala
#include "game.h"
#include "app.h"
#include "game_properties.h"
#include "platform.h"

namespace mandala
{
	game_t::game_t(const std::string& name) :
		name(name)
	{
	}

	void game_t::app_run_start()
	{
		auto properties_path = ".\\resources\\" + name + ".app";

		auto ifstream = std::ifstream(properties_path, std::ios_base::in | std::ios_base::binary);

		properties = game_properties_t(ifstream);

		platform.set_window_title(properties.name);

		for (auto pack : properties.resources.packs)
		{
			app.resources.mount(".\\resources\\" + pack);
		}

		platform.set_window_size(properties.window.size);
		//platform.set_window_position(properties.window.position);
	}

	void game_t::app_run_end()
	{
	}

	void game_t::app_tick_start()
	{
	}

	void game_t::app_tick_end()
	{
	}

	void game_t::app_render_start()
	{
	}

	void game_t::app_render_end()
	{
	}
};
