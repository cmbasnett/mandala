//naga
#include "gpu_buffer.hpp"
#include "gpu.hpp"

namespace naga
{
	GpuBuffer::GpuBuffer()
    {
        id = gpu.create_buffer();
    }

	GpuBuffer::~GpuBuffer()
    {
        gpu.destroy_buffer(id);
    }
}
