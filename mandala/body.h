#pragma once

//mandala
#include "types.hpp"

namespace mandala
{
    namespace armada
    {
        using namespace boost::units;

        struct body_t
        {
            typedef quantity<si::mass, float64_t> mass_type;
            typedef quantity<si::length, float64_t> length_type;

            mass_type mass = 0;
            length_type radius = 0;
        };
    }
}