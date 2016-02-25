#pragma once

//std
#include <bitset>

//naga
#include "terrain.hpp"

namespace naga
{
    struct terrain_chunk
    {
        typedef std::bitset<256> holes_type;
        typedef aabb3 bounds_type;

        holes_type patch_holes;
        bounds_type bounds;
    };
}