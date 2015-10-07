//mandala
#include "gpu_buffer_mgr.hpp"

namespace mandala
{
    gpu_buffer_mgr gpu_buffers;

    void gpu_buffer_mgr::purge()
    {
#if defined(DEBUG)
        //TODO: for some reason, python objects are holding on to these buffers causing the assertion to fail
        for (const auto& buffer : buffers)
        {
            //assert(buffer.second.unique());
        }
#endif

        buffers.clear();
    }
}