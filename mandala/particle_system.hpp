#pragma once

//mandala
#include "resource.hpp"

namespace mandala
{
    struct particle_emitter_t;

    struct particle_system_t : resource_t
    {
        particle_system_t(std::istream& istream);

        std::vector<particle_emitter_t> emitters;
    };
}