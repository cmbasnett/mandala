#pragma once

#include "types.hpp"

//std
#include <type_traits>
#include <cstdint>

namespace mandala
{
    template <std::uintmax_t N>
    struct index_type
    {
        using type = std::conditional_t < N <= 255, uint8_t,
            std::conditional_t < N <= 63535, uint16_t,
            std::conditional_t < N <= 4294967295, uint32_t,
            std::conditional_t < N <= 18446744073709551615ULL, uint64_t,
            std::uintmax_t >> >> ;
    };

    template <std::uintmax_t N>
    using index_type_t = typename index_type<N>::type;
};
