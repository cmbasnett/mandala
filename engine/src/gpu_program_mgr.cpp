//naga
#include "gpu_program_mgr.hpp"

namespace naga
{
	GpuProgramManager gpu_programs;

	void GpuProgramManager::purge()
    {
#if defined(DEBUG)
        for (auto& gpu_program : gpu_programs)
        {
            assert(gpu_program.second.unique());
        }
#endif
        gpu_programs.clear();
    }
}