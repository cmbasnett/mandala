#pragma once

//std
#include <istream>
#include <vector>

//mandala
#include "resource.hpp"
#include "color_types.hpp"

namespace mandala
{
	struct image_t : resource_t
	{
        typedef std::vector<uint8_t> data_type;
        typedef int32_t bit_depth_type;
        typedef glm::detail::tvec2<uint32_t> size_type;

		image_t() = default;
		image_t(std::istream& istream);
        image_t(const size_type& size, bit_depth_type bit_depth, color_type_e color_type, const data_type::iterator& data_begin, const data_type::iterator& data_end);

        bit_depth_type get_bit_depth() const { return bit_depth; }
        color_type_e get_color_type() const { return color_type; }
        const data_type& get_data() const { return data; }
        const size_type& get_size() const { return size; }

	private:
		image_t(const image_t&) = delete;
		image_t& operator=(const image_t&) = delete;

        size_type size;
        int32_t bit_depth = 0;
        color_type_e color_type = color_type_e::g;
        data_type data;
	};
}
