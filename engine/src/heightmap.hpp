#pragma once

// std
#include <type_traits>
#include <vector>

// boost
#include <boost/multi_array.hpp>

// naga
#include "types.hpp"

namespace naga
{
    struct image;

    struct heightmap
    {
        heightmap(const boost::shared_ptr<image>& image);
        heightmap(size_t width, size_t depth) :
			width(width),
			depth(depth),
            data(boost::extents[width][depth])
        {
        }

        size_t get_width() const { return width; }
        size_t get_depth() const { return depth; }
        f32 get_data(size_t x, size_t z) const { return data[z][x]; }
        f32 get_height(f32 x, f32 z) const;
        f32 get_height(const vec2& location) const;
        const f32* get_data() const { return data.data(); }

    private:
		size_t width;
		size_t depth;
        boost::multi_array<f32, 2> data;
    };
}
