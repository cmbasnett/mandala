#pragma once

//boost
#include <boost/enable_shared_from_this.hpp>

//mandala
#include "gpu_defs.hpp"

namespace mandala
{
    struct gpu_buffer : boost::enable_shared_from_this<gpu_buffer>
    {
        gpu_id get_id() const { return id; }

        virtual ~gpu_buffer();

    protected:
        gpu_buffer();

    private:
        gpu_id id;
    };

    template<typename T, typename Enable = void>
    struct is_gpu_buffer : std::false_type
    {
    };

    template<typename T>
    struct is_gpu_buffer<T, typename std::enable_if<std::is_base_of<gpu_buffer, T>::value>::type> : std::true_type
    {
    };
}
