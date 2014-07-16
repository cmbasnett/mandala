#pragma once

//std
#include <array>
#include <vector>

//mandala
#include "types.hpp"
#include "gpu_buffer.hpp"
#include "gpu_mgr.hpp"

namespace mandala
{
    template<typename T>
    struct vertex_buffer_t : gpu_buffer_t
    {
        typedef T vertex_type;
        
        static const auto vertex_size = sizeof(vertex_type);

        vertex_buffer_t()
        {
        }

        void data(vertex_type* vertices, std::size_t count, gpu_mgr_t::buffer_usage_e usage)
        {
            gpu.buffers.push(gpu_mgr_t::buffer_target_e::array, shared_from_this());
            gpu.buffers.data(gpu_mgr_t::buffer_target_e::array, static_cast<void*>(vertices), vertex_size * count, usage);
            gpu.buffers.pop(gpu_mgr_t::buffer_target_e::array);
        }

        template<std::uintmax_t N>
        void data(std::array<vertex_type, N> vertices, gpu_mgr_t::buffer_usage_e usage)
        {
            data(vertices.data(), N, usage);
        }

        void data(std::vector<vertex_type> vertices, gpu_mgr_t::buffer_usage_e usage)
        {
            data(vertices.data(), vertices.size(), usage);
        }

        vertex_buffer_t(const vertex_buffer_t&) = delete;
        vertex_buffer_t& operator=(const vertex_buffer_t&) = delete;
    };
}
