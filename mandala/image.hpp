#pragma once

//std
#include <fstream>
#include <vector>

//mandala
#include "resource.hpp"
#include "color_types.hpp"

namespace mandala
{
	struct image_t : resource_t
	{
		uint32_t width = 0;
		uint32_t height = 0;
		int32_t bit_depth = 0;
        color_type_e color_type = color_type_e::g;
		std::vector<uint8_t> data;

		image_t();
		image_t(std::istream& istream);

	private:
		image_t(const image_t&) = delete;
		image_t& operator=(const image_t&) = delete;
	};
}
