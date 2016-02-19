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
    struct heightmap
    {
        heightmap(size_t width, size_t depth) :
			width(width),
			depth(depth),
            outer(boost::extents[width + 1][depth + 1]),
            inner(boost::extents[width][depth])
        {
        }

		size_t get_width() const { return width; }
		size_t get_depth() const { return depth; }
		f32 get_height(f32 x, f32 y) const;
		f32 get_height(const vec2& location) const;

		//TODO: get height @ location

    private:
		size_t width;
		size_t depth;
        boost::multi_array<f32, 2> outer;
        boost::multi_array<f32, 2> inner;
    };
}