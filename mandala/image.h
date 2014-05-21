#pragma once

//std
#include <fstream>
#include <vector>

//mandala
#include "mandala.h"
#include "resource.h"

namespace mandala
{
	struct image_t : resource_t
	{
		enum color_type_t : int32_t
		{
			g,
			ga,
			rgb,
			rgba,
			palette
		};

		uint32_t width = 0;
		uint32_t height = 0;
		int32_t bit_depth = 0;
		color_type_t color_type = color_type_t::g;
		std::vector<uint8_t> data;

		image_t();
		image_t(std::istream& istream);

	private:
		image_t(const image_t&) = delete;
		image_t& operator=(const image_t&) = delete;
	};
};
