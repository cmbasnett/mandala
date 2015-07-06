#pragma once

//std
#include <array>
#include <vector>

//mandala
#include "types.hpp"
#include "gpu_buffer.hpp"
#include "gpu.hpp"

namespace mandala
{
    template<typename Vertex>
    struct vertex_buffer_t : gpu_buffer_t
    {
        typedef Vertex vertex_type;
        
        static const auto VERTEX_SIZE = sizeof(vertex_type);

        vertex_buffer_t() = default;

        void data(const vertex_type* vertices, std::size_t count, gpu_t::buffer_usage_e usage)
        {
            gpu.buffers.push(gpu_t::buffer_target_e::ARRAY, shared_from_this());
            gpu.buffers.data(gpu_t::buffer_target_e::ARRAY, static_cast<const void*>(vertices), VERTEX_SIZE * count, usage);
            gpu.buffers.pop(gpu_t::buffer_target_e::ARRAY);
        }

        void data(std::initializer_list<vertex_type>& vertices, gpu_t::buffer_usage_e usage)
        {
            data(vertices.begin(), vertices.size(), usage);
        }

		void data(std::vector<vertex_type>& vertices, gpu_t::buffer_usage_e usage)
        {
            data(vertices.data(), vertices.size(), usage);
        }

	private:
        vertex_buffer_t(const vertex_buffer_t&) = delete;
        vertex_buffer_t& operator=(const vertex_buffer_t&) = delete;
    };
}
