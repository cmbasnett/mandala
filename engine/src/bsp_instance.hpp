#pragma once

#include "actor.hpp"

namespace mandala
{
    struct bsp_t;

    struct bsp_instance_t : actor_t
    {
        bsp_instance_t(const boost::shared_ptr<bsp_t>& bsp);

    private:
        boost::shared_ptr<bsp_t> bsp;
    };
}