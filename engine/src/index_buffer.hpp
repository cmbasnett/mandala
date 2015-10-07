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
    struct index_buffer;

    //TODO: is_integral is not actually restrictive enough, since int64s can't be translated to a GL type
    template<typename T>
    struct index_buffer<T, typename std::enable_if<std::is_integral<T>::value>::type> : gpu_buffer
    {
        typedef T index_type;

        static const auto DATA_TYPE = gpu_data_type_<index_type>::VALUE;

        index_buffer() = default;

        void data(const index_type* indices, size_t count, gpu_t::buffer_usage usage)
        {
            gpu.buffers.push(gpu_t::buffer_target::ELEMENT_ARRAY, shared_from_this());
            gpu.buffers.data(gpu_t::buffer_target::ELEMENT_ARRAY, static_cast<const void*>(indices), count * sizeof(index_type), usage);
            gpu.buffers.pop(gpu_t::buffer_target::ELEMENT_ARRAY);
        }

        void data(const std::initializer_list<index_type> indices, gpu_t::buffer_usage usage)
        {
            data(indices.begin(), indices.size(), usage);
        }

        void data(const std::vector<index_type> indices, gpu_t::buffer_usage usage)
        {
            data(indices.data(), indices.size(), usage);
        }

    private:
        index_buffer(const index_buffer&) = delete;
        index_buffer& operator=(const index_buffer&) = delete;
    };
}
