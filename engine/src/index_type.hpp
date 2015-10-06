#pragma once

//mandala
#include "types.hpp"

//std
#include <type_traits>
#include <cstdint>

namespace mandala
{
    template <std::uintmax_t N>
    struct index_type
    {
        using type = std::conditional_t < N <= 255, u8,
            std::conditional_t < N <= 63535, u16,
            std::conditional_t < N <= 4294967295, u32,
            std::conditional_t < N <= 18446744073709551615ULL, u64,
            std::uintmax_t >> >> ;
    };
}
