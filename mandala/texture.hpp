#pragma once

//std
#include <memory>

//mandala
#include "resource.hpp"
#include "color_types.hpp"

namespace mandala
{
    struct image_t;

	struct texture_t : resource_t, std::enable_shared_from_this<texture_t>
	{
		typedef uint32_t size_value_type;
        typedef glm::detail::tvec2<size_value_type> size_type;
		typedef uint32_t id_type;
		typedef int32_t format_type;
		typedef int32_t type_type;

        texture_t(color_type_e color_type, const size_type& size, const void* data);
		texture_t(const std::shared_ptr<image_t>& image);
		texture_t(std::istream& istream);
		virtual ~texture_t();

        color_type_e get_color_type() const { return color_type; }
        const size_type& get_size() const { return size; }
        id_type get_id() const { return id; }

        void set_size(const size_type& size);

	private:
        color_type_e color_type;
        size_type size;
        id_type id = 0;

		texture_t(texture_t&) = delete;
		texture_t& operator=(texture_t&) = delete;
	};
}
