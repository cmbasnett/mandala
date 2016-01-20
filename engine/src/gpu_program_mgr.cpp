//naga
#include "gpu_program_mgr.hpp"

namespace naga
{
    gpu_program_mgr gpu_programs;

    void gpu_program_mgr::purge()
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