#pragma once

//std
#include <ratio>

namespace naga
{
    constexpr long double operator"" _deg(long double deg)
    {
        return deg * (3.141592 / 180);
    }

    constexpr long double operator"" _rad(long double rad)
    {
        return rad * (180 / 3.141592);
    }

    constexpr auto d = 360.0_deg;
    constexpr auto r = 1.432_rad;
}
