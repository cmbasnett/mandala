//naga
#include "gpu_program.hpp"
#include "gpu.hpp"

namespace naga
{
    gpu_program::gpu_program(const std::string& vertex_shader_source, const std::string& fragment_shader_source)
    {
        id = gpu.create_program(vertex_shader_source, fragment_shader_source);
    }

    gpu_program::~gpu_program()
    {
        gpu.destroy_program(id);
    }
}
