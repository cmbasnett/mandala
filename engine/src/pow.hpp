#pragma once

//std
#include <type_traits>

namespace naga
{
    namespace type_traits
    {
        template<int Base, int Exponent>
        struct pow
        {
            static const int VALUE = Base * pow<Base, Exponent - 1>::value;
        };

        template<int Base>
        struct pow<Base, 0>
        {
            static const int VALUE = 1;
        };
    };
}
