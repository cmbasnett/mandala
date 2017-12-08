#pragma once

//boost
#include <boost/enable_shared_from_this.hpp>

//naga
#include "gpu_defs.hpp"

namespace naga
{
	struct GpuBuffer : boost::enable_shared_from_this<GpuBuffer>
    {
        GpuId get_id() const { return id; }

		virtual ~GpuBuffer();

    protected:
		GpuBuffer();

    private:
		GpuId id;
    };

    template<typename T, typename Enable = void>
    struct IsGpuBuffer : std::false_type
    {
    };

    template<typename T>
	struct IsGpuBuffer<T, typename std::enable_if<std::is_base_of<GpuBuffer, T>::value>::type> : std::true_type
    {
    };
}
