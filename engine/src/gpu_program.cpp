//naga
#include "gpu_program.hpp"
#include "gpu.hpp"

namespace naga
{
	GpuProgram::GpuProgram(const std::string& vertex_shader_source, const std::string& fragment_shader_source)
    {
        id = gpu.create_program(vertex_shader_source, fragment_shader_source);
    }

	GpuProgram::~GpuProgram()
    {
        gpu.destroy_program(id);
    }
}
