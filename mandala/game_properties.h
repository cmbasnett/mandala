#pragma once

//std
#include <vector>

//mandala
#include "resource.h"

namespace mandala
{
	struct game_properties_t
	{
		struct resources_t
		{
			std::vector<std::string> packs;
		};

		struct window_t
		{
			typedef vec2_i32_t size_type_t;
			typedef vec2_i32_t position_type_t;

			size_type_t size;
			position_type_t position;
		};

		std::string name;
		window_t window;
		resources_t resources;

		game_properties_t();
		game_properties_t(std::istream& istream);
	};
};
