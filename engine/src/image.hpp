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
#include "types.hpp"

namespace naga
{
	struct Image : Resource
    {
        typedef std::vector<u8> DataType;
        typedef i32 BitDepthType;
        typedef vec2 SizeType;

		Image() = default;
		Image(std::istream& istream);
		Image(const SizeType& size, BitDepthType bit_depth, ColorType color_type, const u8* data_ptr, size_t data_size);

		BitDepthType get_bit_depth() const { return bit_depth; }
		ColorType get_color_type() const { return color_type; }
		const DataType& get_data() const { return data; }
		const SizeType& get_size() const { return size; }
		u32 get_width() const { return static_cast<u32>(size.x); }
        u32 get_height() const { return static_cast<u32>(size.y); }
        size_t get_pixel_stride() const { return pixel_stride; }
		size_t get_channel_count() const;
        std::mutex& get_data_mutex() { return data_mutex; }

    private:
		Image(const Image&) = delete;
		Image& operator=(const Image&) = delete;

        SizeType size;
        BitDepthType bit_depth = 0;
		ColorType color_type = ColorType::G;
        DataType data;
        size_t pixel_stride = 1;
        std::mutex data_mutex;

		friend std::ostream& operator<<(std::ostream& ostream, Image& image);
    };
}
