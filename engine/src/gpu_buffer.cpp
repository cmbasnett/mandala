//naga
#include "gpu_buffer.hpp"
#include "gpu.hpp"

namespace naga
{
    gpu_buffer::gpu_buffer()
    {
        id = gpu.create_buffer();
    }

    gpu_buffer::~gpu_buffer()
    {
        gpu.destroy_buffer(id);
    }
}
