//mandala
#include "gpu_program_mgr.hpp"

namespace mandala
{
	gpu_program_mgr_t gpu_programs;

    void gpu_program_mgr_t::purge()
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