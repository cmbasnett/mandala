//mandala
#include "gpu_buffer.hpp"
#include "gpu.hpp"

namespace mandala
{
    gpu_buffer_t::gpu_buffer_t()
    {
		id = gpu.create_buffer();
    }

    gpu_buffer_t::~gpu_buffer_t()
    {
		gpu.destroy_buffer(id);
    }
}
