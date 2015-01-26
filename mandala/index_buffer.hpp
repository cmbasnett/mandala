#pragma once

//std
#include <array>
#include <vector>
#include <type_traits>

//mandala
#include "types.hpp"
#include "gpu_buffer.hpp"

namespace mandala
{
    template<typename T, typename Enable = void>
    struct index_buffer_t;

    template<typename T>
    struct index_buffer_t<T, typename std::enable_if<std::is_integral<T>::value>::type> : gpu_buffer_t
    {
        typedef T index_type;
        static const auto index_size = sizeof(index_type);

        void data(const index_type* indices, size_t count, gpu_t::buffer_usage_e usage)
        {
            gpu.buffers.push(gpu_t::buffer_target_e::element_array, shared_from_this());
            gpu.buffers.data(gpu_t::buffer_target_e::element_array, static_cast<const void*>(indices), count * index_size, usage);
            gpu.buffers.pop(gpu_t::buffer_target_e::element_array);
        }

        void data(const std::initializer_list<index_type> indices, gpu_t::buffer_usage_e usage)
        {
            data(indices.begin(), indices.size(), usage);
        }

        void data(const std::vector<index_type> indices, gpu_t::buffer_usage_e usage)
        {
            data(indices.data(), indices.size(), usage);
        }
    };
}
