#pragma once

//mandala
#include "resource.hpp"
#include "range.hpp"

namespace mandala
{
    struct particle_emitter_t : resource_t
    {
        enum class start_shape_e
        {
            point,
            sphere,
            aabb,
        };

        enum class movement_space_e
        {
            world,
            local
        };

        start_shape_e start_shape = start_shape_e::point;
        movement_space_e movement_space = movement_space_e::world;
        range_t<float32_t> frequency_range;
        range_t<size_t> emissions_max_range;
    };
}