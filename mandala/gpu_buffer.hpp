#pragma once

//std
#include <memory>

//mandala
#include "gpu_defs.hpp"

namespace mandala
{
    struct gpu_buffer_t : std::enable_shared_from_this<gpu_buffer_t>
    {
        gpu_id_t get_id() const { return id; }

        virtual ~gpu_buffer_t();

    protected:
        gpu_buffer_t();

    private:
        gpu_id_t id;
    };

    template<typename T, typename Enable = void>
    struct is_gpu_buffer : std::false_type
    {
    };

    template<typename T>
    struct is_gpu_buffer<T, typename std::enable_if<std::is_base_of<gpu_buffer_t, T>::value>::type> : std::true_type
    {
    };
}
