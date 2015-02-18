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

    //TODO: is_integral is not actually restrictive enough, since int64s can't be translated to a GL type
    template<typename T>
    struct index_buffer_t<T, typename std::enable_if<std::is_integral<T>::value>::type> : gpu_buffer_t
    {
        typedef T index_type;

        static const auto data_type = gpu_data_type_<index_type>::value;

        index_buffer_t() = default;

        void data(const index_type* indices, size_t count, gpu_t::buffer_usage_e usage)
        {
            gpu.buffers.push(gpu_t::buffer_target_e::element_array, shared_from_this());
            gpu.buffers.data(gpu_t::buffer_target_e::element_array, static_cast<const void*>(indices), count * sizeof(index_type), usage);
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

    private:
        index_buffer_t(const index_buffer_t&) = delete;
        index_buffer_t& operator=(const index_buffer_t&) = delete;
    };
}
