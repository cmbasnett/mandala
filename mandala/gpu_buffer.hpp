#pragma once

//std
#include <memory>

//mandala
#include "types.hpp"

namespace mandala
{
    struct gpu_buffer_t : std::enable_shared_from_this<gpu_buffer_t>
    {
        typedef uint32_t id_type;

        id_type id = 0;

        virtual ~gpu_buffer_t();

    protected:
        gpu_buffer_t();
    };
}
