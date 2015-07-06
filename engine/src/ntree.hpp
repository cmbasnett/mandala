#pragma once

#include "is_pow2.hpp"

#include <array>
#include <boost/shared_ptr.hpp>

namespace mandala
{
    template<size_t N>
    struct ntree
    {
        typedef ntree<N> type;

        std::array<boost::shared_ptr<type>, N> children;

        ntree() = default;
    };
}