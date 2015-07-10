#pragma once

//std
#include <type_traits>

namespace mandala
{
    template<size_t N>
    struct is_pow2
    {
        static const bool VALUE = (N != 0 && (N & (~N + 1)) == N);
    };
}
