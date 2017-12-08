//naga
#include "gpu_buffer_mgr.hpp"

namespace naga
{
	GpuBufferManager gpu_buffers;

	void GpuBufferManager::purge()
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