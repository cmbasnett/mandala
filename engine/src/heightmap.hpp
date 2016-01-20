#pragma once

// std
#include <type_traits>
#include <vector>

// boost
#include <boost/multi_array.hpp>

// naga
#include "types.hpp"
#include "collision.hpp"

namespace naga
{
    struct heightmap
    {
        heightmap(size_t width, size_t depth) :
            outer(boost::extents[width + 1][depth + 1]),
            inner(boost::extents[width][depth])
        {
        }

    private:
        boost::multi_array<f32, 2> outer;
        boost::multi_array<f32, 2> inner;
    };
}