#pragma once

//std
#include <istream>
#include <vector>
#include <mutex>

//boost
#include <boost\shared_ptr.hpp>

//naga
#include "resource.hpp"
#include "color_types.hpp"

namespace naga
{
    struct image : resource
    {
        typedef std::vector<u8> data_type;
        typedef i32 bit_depth_type;
        typedef glm::detail::tvec2<u32> size_type;

        image() = default;
        image(std::istream& istream);
        image(const size_type& size, bit_depth_type bit_depth, color_type color_type, const data_type::value_type* data_ptr, size_t data_size);

        bit_depth_type get_bit_depth() const { return bit_depth; }
        color_type get_color_type() const { return color_type; }
        const data_type& get_data() const { return data; }
        const size_type& get_size() const { return size; }
        std::mutex& get_data_mutex() { return data_mutex; }

    private:
        image(const image&) = delete;
        image& operator=(const image&) = delete;

        size_type size;
        i32 bit_depth = 0;
        color_type color_type = color_type::G;
        data_type data;
        std::mutex data_mutex;
    };
}

std::ostream& operator<<(std::ostream& ostream, naga::image& image);
