//libpng
#include <png.h>
#include <iostream>

//naga
#include "image.hpp"

namespace naga
{
	Image::Image(std::istream& istream)
    {
        //TODO: determine what the stream actually contains (don't assume PNG!)
        auto png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

        if (png_ptr == nullptr)
        {
            throw std::exception();
        }

        auto info_ptr = png_create_info_struct(png_ptr);

        if (info_ptr == nullptr)
        {
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);

            throw std::exception();
        }

        if (setjmp(png_jmpbuf(png_ptr)))
        {
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

            throw std::exception();
        }

        png_set_read_fn(png_ptr, static_cast<png_voidp>(&istream), [](png_structp png_ptr, png_bytep data, png_size_t length)
        {
            auto png_io_ptr = png_get_io_ptr(png_ptr);

            static_cast<std::istream*>(png_io_ptr)->read(reinterpret_cast<char*>(data), length);
        });

        png_uint_32 sig_read = 0;

        png_set_sig_bytes(png_ptr, sig_read);
        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, nullptr);

        int png_color_type;
        png_int_32 interlace_method;
		png_uint_32 width, height;

        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &png_color_type, &interlace_method, nullptr, nullptr);

		size.x = static_cast<float>(width);
		size.y = static_cast<float>(height);

		auto get_color_type = [](int png_color_type) -> ColorType
        {
            switch (png_color_type)
            {
            case PNG_COLOR_TYPE_GRAY:
                return ColorType::G;
            case PNG_COLOR_TYPE_RGB:
				return ColorType::RGB;
            case PNG_COLOR_TYPE_PALETTE:
				return ColorType::PALETTE;
            case PNG_COLOR_TYPE_GA:
				return ColorType::GA;
            case PNG_COLOR_TYPE_RGBA:
				return ColorType::RGBA;
            default:
                throw std::exception();
            }
        };

        color_type = get_color_type(png_color_type);

        auto row_bytes = png_get_rowbytes(png_ptr, info_ptr);

        pixel_stride = row_bytes / get_width();

        auto data_length = row_bytes * get_height();

        data.resize(data_length);

        auto row_pointers = png_get_rows(png_ptr, info_ptr);

        for (u32 i = 0; i < size.y; ++i)
        {
            memcpy_s(data.data() + (row_bytes * (get_height() - 1 - i)), row_bytes, row_pointers[i], row_bytes);
        }

        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }

	Image::Image(const SizeType& size, BitDepthType bit_depth, ColorType color_type, const u8* data_ptr, size_t data_size) :
        size(size),
        bit_depth(bit_depth),
        color_type(color_type)
    {
        data.resize(data_size);

		// TODO: 

        memcpy_s(data.data(), data.size(), data_ptr, data_size);
    }


	size_t Image::get_channel_count() const
	{
		switch (color_type)
		{
		case ColorType::G:
		case ColorType::PALETTE:
			return 1;
		case ColorType::GA:
			return 2;
		case ColorType::RGB:
			return 3;
		case ColorType::RGBA:
			return 4;
		default:
			return 0;
		}
	}

	// TODO: ugly implementation
	std::string png_error_message;

	std::ostream& operator<<(std::ostream& ostream, Image& image)
	{
		auto png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

		if (!png_ptr)
		{
			throw std::exception();
		}

		auto info_ptr = png_create_info_struct(png_ptr);

		if (!info_ptr)
		{
			png_destroy_write_struct(&png_ptr, nullptr);

			throw std::exception();
		}

		std::exception exception;

		png_error_ptr error_fn = [](png_structp png_ptr, png_const_charp message) {
			png_error_message = message;
		};

		png_set_error_fn(png_ptr, png_get_error_ptr(png_ptr), error_fn, NULL);

		if (setjmp(png_jmpbuf(png_ptr)))
		{
			auto error_ptr = png_get_error_ptr(png_ptr);
			png_destroy_write_struct(&png_ptr, &info_ptr);
			throw std::exception(png_error_message.c_str());
		}

		auto get_png_color_type = [](ColorType color_type) -> int
		{
			switch (color_type)
			{
			case ColorType::G:
				return PNG_COLOR_TYPE_GRAY;
			case ColorType::RGB:
				return PNG_COLOR_TYPE_RGB;
			case ColorType::PALETTE:
				return PNG_COLOR_TYPE_PALETTE;
			case ColorType::GA:
				return PNG_COLOR_TYPE_GA;
			case ColorType::RGBA:
				return PNG_COLOR_TYPE_RGBA;
			case ColorType::DEPTH_STENCIL:
				return PNG_COLOR_TYPE_RGBA;
			default:
				throw std::exception();
			}
		};

		auto png_color_type = get_png_color_type(image.get_color_type());

		png_set_IHDR(png_ptr,
			info_ptr,
			image.get_width(),
			image.get_height(),
			image.get_bit_depth(),
			png_color_type,
			PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT,
			PNG_FILTER_TYPE_DEFAULT);

		std::unique_lock<std::mutex> image_data_lock(image.get_data_mutex());
		auto data_ptr = image.get_data().data();

		png_bytepp rows = static_cast<png_bytepp>(png_malloc(png_ptr, image.get_height() * sizeof(png_bytep)));

		const auto bytes_per_pixel = glm::max(1u, (image.get_bit_depth() * image.get_channel_count() / 8));
		const auto row_size = (image.get_width() * bytes_per_pixel);

		for (unsigned int y = 0; y < image.get_size().y; ++y)
		{
			auto row = static_cast<png_bytep>(png_malloc(png_ptr, row_size));

			for (unsigned int x = 0; x < image.get_size().x; ++x)
			{
				memcpy_s(row, row_size, data_ptr + (row_size * y), row_size);
			}

			rows[image.get_height() - y - 1] = row;
		}

		image_data_lock.unlock();

		png_set_rows(png_ptr, info_ptr, rows);
		png_set_write_fn(png_ptr, static_cast<png_voidp>(&ostream), [](png_structp png_ptr, png_bytep data, png_size_t size)
		{
			static_cast<std::ostream*>(png_get_io_ptr(png_ptr))->write(reinterpret_cast<char*>(data), size);
		}, nullptr);
		png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);

		return ostream;
	}
}