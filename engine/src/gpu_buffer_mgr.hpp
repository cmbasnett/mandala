#pragma once

//std
#include <map>

//boost
#include <boost\make_shared.hpp>

//naga
#include "gpu_buffer.hpp"

namespace naga
{
    struct gpu_buffer_mgr
    {
        template<typename T = std::enable_if<is_gpu_buffer<T>::value, T>::type>
        boost::weak_ptr<T> make()
        {
            auto gpu_buffer = boost::make_shared<T>();

            auto buffers_itr = buffers.insert(buffers.begin(), std::make_pair(gpu_buffer->get_id(), gpu_buffer));

            return boost::static_pointer_cast<T, naga::gpu_buffer>(buffers_itr->second);
        }

        void purge();

    private:
        std::map<gpu_id, boost::shared_ptr<gpu_buffer>> buffers;
    };

    extern gpu_buffer_mgr gpu_buffers;
}