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
		typedef uint32_t size_type;
		typedef uint32_t id_type;
		typedef int32_t format_type;
		typedef int32_t type_type;

		size_type width = 0;
		size_type height = 0;
		id_type id = 0;
		format_type internal_format = 0;
		format_type format = 0;
		type_type type = 0;

		texture_t(color_type_e color_type, size_type width, size_type height);
        texture_t(color_type_e color_type, vec2_u32_t size);
		texture_t(const std::shared_ptr<image_t>& image);
		texture_t(std::istream& istream);
		virtual ~texture_t();

	private:
		texture_t(texture_t&) = delete;
		texture_t& operator=(texture_t&) = delete;
	};
}
