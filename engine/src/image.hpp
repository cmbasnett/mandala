#pragma once

//std
#include <istream>
#include <vector>
#include <mutex>

//boost
#include <boost\shared_ptr.hpp>

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
        image_t(const size_type& size, bit_depth_type bit_depth, color_type_e color_type, const data_type::value_type* data_ptr, size_t data_size);

        bit_depth_type get_bit_depth() const { return bit_depth; }
        color_type_e get_color_type() const { return color_type; }
        const data_type& get_data() const { return data; }
        const size_type& get_size() const { return size; }
        std::mutex& get_data_mutex() { return data_mutex; }

	private:
		image_t(const image_t&) = delete;
		image_t& operator=(const image_t&) = delete;

        size_type size;
        int32_t bit_depth = 0;
        color_type_e color_type = color_type_e::g;
        data_type data;
        std::mutex data_mutex;
	};
}

std::ostream& operator<<(std::ostream& ostream, mandala::image_t& image);
