#pragma once

//armada
#include "body.h"

//glm
#include <glm\ext.hpp>

//boost
#include <boost\units\systems\si.hpp>

namespace mandala
{
    namespace armada
    {
        using namespace boost::units;

        typedef quantity<si::force, float64_t> force_type;

        static const auto G = 6.67384e-11;

        static inline force_type get_body_force(const body_t& b1, const body_t& b2)
        {
            auto r2 = 1.0;

            return force_type(G * ((b1.mass * b2.mass) / r2));
        }
    }
}
