//mandala
#include "gpu_program.hpp"
#include "gpu.hpp"

namespace mandala
{
    gpu_program_t::gpu_program_t(const std::string& vertex_shader_source, const std::string& fragment_shader_source)
    {
        id = gpu.create_program(vertex_shader_source, fragment_shader_source);
    }

    gpu_program_t::~gpu_program_t()
    {
        gpu.destroy_program(id);
    }
}
