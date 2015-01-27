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
		typedef uint32_t size_value_type;
        typedef glm::detail::tvec2<size_value_type> size_type;
		typedef uint32_t id_type;
		typedef int32_t format_type;
		typedef int32_t type_type;

        size_value_type width = 0;
        size_value_type height = 0;
		id_type id = 0;
		format_type internal_format = 0;
		format_type format = 0;
		type_type type = 0;

		texture_t(color_type_e color_type, size_value_type width, size_value_type height, const void* data);
        texture_t(color_type_e color_type, size_type size, const void* data);
		texture_t(const std::shared_ptr<image_t>& image);
		texture_t(std::istream& istream);
		virtual ~texture_t();

	private:
		texture_t(texture_t&) = delete;
		texture_t& operator=(texture_t&) = delete;
	};
}
