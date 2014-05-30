//libpng
#include <png.h>

//mandala
#include "image.h"

namespace mandala
{
	image_t::image_t()
	{
	}

	image_t::image_t(std::istream& istream)
	{
		png_structp png_ptr;
		png_infop info_ptr;
		png_uint_32 sig_read = 0;

		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

		if (png_ptr == nullptr)
		{
			throw std::exception();
		}

		info_ptr = png_create_info_struct(png_ptr);

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

			static_cast<std::istream*>(png_io_ptr)->read((char*)data, length);
		});

		png_set_sig_bytes(png_ptr, sig_read);
		png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, nullptr);

		int png_color_type = 0;

		png_int_32 interlace_type;
		png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &png_color_type, &interlace_type, nullptr, nullptr);

		switch (png_color_type)
		{
			case 0:
				color_type = color_type_t::g;
				break;
			case 2:
				color_type = color_type_t::rgb;
				break;
			case 3:
				color_type = color_type_t::palette;
				break;
			case 4:
				color_type = color_type_t::ga;
				break;
			case 6:
				color_type = color_type_t::rgba;
				break;
			default:
				throw std::exception();
		}

		auto row_bytes = png_get_rowbytes(png_ptr, info_ptr);
		auto data_length = row_bytes * height;

		data.reserve(data_length);

		auto row_pointers = png_get_rows(png_ptr, info_ptr);

		for (uint32_t i = 0; i < height; ++i)
		{
			memcpy(data.data() + (row_bytes * (height - 1 - i)), row_pointers[i], row_bytes);
		}

		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
	}
};