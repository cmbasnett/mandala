#pragma once

//std
#include <array>
#include <vector>
#include <type_traits>

//naga
#include "types.hpp"
#include "gpu_buffer.hpp"

namespace naga
{
	template<typename Index, typename Enable = void>
    struct IndexBuffer;

    //TODO: is_integral is not actually restrictive enough, since int64s can't be translated to a GL type
    template<typename Index>
	struct IndexBuffer<Index, typename std::enable_if<std::is_integral<Index>::value>::type> : GpuBuffer
    {
		typedef Index IndexType;

		static const auto DATA_TYPE = GpuDataType<IndexType>::VALUE;

		IndexBuffer() = default;

		void data(const IndexType* indices, size_t count, Gpu::BufferUsage usage)
        {
            gpu.buffers.push(Gpu::BufferTarget::ELEMENT_ARRAY, shared_from_this());
			gpu.buffers.data(Gpu::BufferTarget::ELEMENT_ARRAY, static_cast<const void*>(indices), count * sizeof(IndexType), usage);
            gpu.buffers.pop(Gpu::BufferTarget::ELEMENT_ARRAY);
        }

		void data(const std::initializer_list<IndexType> indices, Gpu::BufferUsage usage)
        {
            data(indices.begin(), indices.size(), usage);
        }

		void data(const std::vector<IndexType> indices, Gpu::BufferUsage usage)
        {
            data(indices.data(), indices.size(), usage);
        }

    private:
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
    };
}
