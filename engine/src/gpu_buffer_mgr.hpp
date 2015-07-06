#pragma once

//std
#include <map>

//mandala
#include "gpu_buffer.hpp"

namespace mandala
{
    struct gpu_buffer_mgr_t
    {
        template<typename T = std::enable_if<is_gpu_buffer<T>::value, T>::type>
        boost::weak_ptr<T> make()
        {
            auto gpu_buffer = boost::make_shared<T>();

            auto buffers_itr = buffers.insert(buffers.begin(), std::make_pair(gpu_buffer->get_id(), gpu_buffer));

            return boost::static_pointer_cast<T, gpu_buffer_t>(buffers_itr->second);
        }

        void purge();

    private:
        std::map<gpu_id_t, boost::shared_ptr<gpu_buffer_t>> buffers;
    };

    extern gpu_buffer_mgr_t gpu_buffers;
}