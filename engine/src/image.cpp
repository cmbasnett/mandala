//libpng
#include <png.h>
#include <iostream>

//naga
#include "image.hpp"

namespace naga
{
    image::image(std::istream& istream)
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

        png_get_IHDR(png_ptr, info_ptr, &size.x, &size.y, &bit_depth, &png_color_type, &interlace_method, nullptr, nullptr);

        auto get_color_type = [](int png_color_type) -> naga::color_type
        {
            switch (png_color_type)
            {
            case PNG_COLOR_TYPE_GRAY:
                return color_type::G;
            case PNG_COLOR_TYPE_RGB:
                return color_type::RGB;
            case PNG_COLOR_TYPE_PALETTE:
                return color_type::PALETTE;
            case PNG_COLOR_TYPE_GA:
                return color_type::GA;
            case PNG_COLOR_TYPE_RGBA:
                return color_type::RGBA;
            default:
                throw std::exception();
            }
        };

        color_type = get_color_type(png_color_type);

        auto row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        auto data_length = row_bytes * size.y;

        data.reserve(data_length);

        auto row_pointers = png_get_rows(png_ptr, info_ptr);

        for (u32 i = 0; i < size.y; ++i)
        {
            memcpy_s(data.data() + (row_bytes * (size.y - 1 - i)), row_bytes, row_pointers[i], row_bytes);
        }

        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }

    image::image(const size_type& size, bit_depth_type bit_depth, naga::color_type color_type, const data_type::value_type* data_ptr, size_t data_size) :
        size(size),
        bit_depth(bit_depth),
        color_type(color_type)
    {
        data.resize(data_size);

        memcpy_s(data.data(), data.size(), data_ptr, data_size);
    }
}

std::ostream& operator<<(std::ostream& ostream, naga::image& image)
{
    using namespace naga;

    auto png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

    if (png_ptr == nullptr)
    {
        throw std::exception();
    }

    auto info_ptr = png_create_info_struct(png_ptr);

    if (!info_ptr)
    {
        png_destroy_write_struct(&png_ptr, nullptr);

        throw std::exception();
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        throw std::exception();
    }

    //TODO: get this thing working with different color types other than rgb
    static const auto PIXEL_SIZE = 3;

    auto get_png_color_type = [](color_type color_type) -> int
    {
        switch (color_type)
        {
        case color_type::G:
            return PNG_COLOR_TYPE_GRAY;
        case color_type::RGB:
            return PNG_COLOR_TYPE_RGB;
        case color_type::PALETTE:
            return PNG_COLOR_TYPE_PALETTE;
        case color_type::GA:
            return PNG_COLOR_TYPE_GA;
        case color_type::RGBA:
            return PNG_COLOR_TYPE_RGBA;
        case color_type::DEPTH_STENCIL:
            return PNG_COLOR_TYPE_RGBA;
        default:
            throw std::exception();
        }
    };

    auto png_color_type = get_png_color_type(image.get_color_type());

    png_set_IHDR(png_ptr,
        info_ptr,
        image.get_size().x,
        image.get_size().y,
        image.get_bit_depth(),
        png_color_type,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);

    std::unique_lock<std::mutex> image_data_lock(image.get_data_mutex());
    auto data_ptr = image.get_data().data();

    png_bytepp rows = static_cast<png_bytepp>(png_malloc(png_ptr, image.get_size().y * sizeof(png_bytep)));

    const auto row_size = image.get_size().x * PIXEL_SIZE;

    for (unsigned int y = 0; y < image.get_size().y; ++y)
    {
        auto row = static_cast<png_bytep>(png_malloc(png_ptr, row_size));

        for (unsigned int x = 0; x < image.get_size().x; ++x)
        {
            memcpy_s(row, row_size, data_ptr + (row_size * y), row_size);
        }

        rows[image.get_size().y - y - 1] = row;
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