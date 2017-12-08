#pragma once

//boost
#include <boost\enable_shared_from_this.hpp>

//naga
#include "types.hpp"
#include "gpu.hpp"

namespace naga
{
	struct GpuProgram : boost::enable_shared_from_this<GpuProgram>
    {
		virtual ~GpuProgram();

        virtual void on_bind() = 0;
        virtual void on_unbind() = 0;

        GpuId get_id() const { return id; }

    protected:
		GpuProgram(const std::string& vertex_shader_source, const std::string& fragment_shader_source);

    private:
		GpuId id;

		GpuProgram(const GpuProgram&) = delete;
		GpuProgram& operator=(const GpuProgram&) = delete;
    };

    template<typename T, typename Enable = void>
    struct IsGpuProgram : std::false_type
    {
    };

    template<typename T>
	struct IsGpuProgram<T, typename std::enable_if<std::is_base_of<GpuProgram, T>::value>::type> : std::true_type
    {
    };
}