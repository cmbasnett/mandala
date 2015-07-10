#pragma once

//std
#include <vector>

//boost
#include <boost/shared_ptr.hpp>

//mandala
#include "quadtree.hpp"

namespace mandala
{
    struct actor_t;

    struct level_t
    {
        std::vector<boost::shared_ptr<actor_t>> actors;
    };

    extern level_t level;
}