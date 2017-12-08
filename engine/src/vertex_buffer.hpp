#pragma once

//std
#include <array>
#include <vector>

//naga
#include "types.hpp"
#include "gpu_buffer.hpp"
#include "gpu.hpp"

namespace naga
{
    template<typename Vertex>
	struct VertexBuffer : GpuBuffer
    {
        typedef Vertex VertexType;
        
		static const auto VERTEX_SIZE = sizeof(VertexType);

		VertexBuffer() = default;

		void data(const VertexType* vertices, std::size_t count, Gpu::BufferUsage usage)
        {
            gpu.buffers.push(Gpu::BufferTarget::ARRAY, shared_from_this());
            gpu.buffers.data(Gpu::BufferTarget::ARRAY, static_cast<const void*>(vertices), VERTEX_SIZE * count, usage);
            gpu.buffers.pop(Gpu::BufferTarget::ARRAY);
        }

		void data(std::initializer_list<VertexType>& vertices, Gpu::BufferUsage usage)
        {
            data(vertices.begin(), vertices.size(), usage);
        }

		void data(std::vector<VertexType>& vertices, Gpu::BufferUsage usage)
        {
            data(vertices.data(), vertices.size(), usage);
        }

    private:
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
    };
}
