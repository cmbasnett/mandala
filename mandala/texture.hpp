#pragma once

//std
#include <memory>

//mandala
#include "resource.hpp"
#include "color_types.hpp"

namespace mandala
{
    struct image_t;

	struct texture_t : resource_t
	{
		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t id = 0;
		int32_t internal_format = 0;
		int32_t format = 0;
        int32_t type = 0;

        texture_t(color_type_e color_type, uint32_t width, uint32_t height);
		texture_t(std::shared_ptr<image_t> image);
		texture_t(std::istream& istream);
		virtual ~texture_t();

        static void get_formats(color_type_e color_type, int32_t& internal_format, int32_t& format, int32_t& type);

	private:
		texture_t(texture_t&) = delete;
		texture_t& operator=(texture_t&) = delete;
	};
}
