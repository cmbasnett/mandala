#pragma once

// std
#include <type_traits>
#include <vector>

// boost
#include <boost/multi_array.hpp>

// mandala
#include "types.hpp"
#include "collision.hpp"

namespace mandala
{
    struct heightmap_t
    {
        heightmap_t(size_t width, size_t depth) :
            outer(boost::extents[width + 1][depth + 1]),
            inner(boost::extents[width][depth])
        {
        }

    private:
        boost::multi_array<f32, 2> outer;
        boost::multi_array<f32, 2> inner;
    };
}