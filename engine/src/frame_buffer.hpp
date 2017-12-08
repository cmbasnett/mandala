#pragma once

//std
#include <map>

//boost
#include <boost\shared_ptr.hpp>

//naga
#include "types.hpp"
#include "gpu_defs.hpp"

namespace naga
{
	struct Texture;

    struct FrameBuffer
    {
		GpuId get_id() const { return id; }
        const boost::shared_ptr<Texture>& get_color_texture() const { return color_texture; }
		const boost::shared_ptr<Texture>& get_depth_texture() const { return depth_texture; }
		const boost::shared_ptr<Texture>& get_depth_stencil_texture() const { return depth_stencil_texture; }
        const GpuFrameBufferSizeType& get_size() const { return size; }
        GpuFrameBufferType get_type() const { return type; }

        void set_size(const GpuFrameBufferSizeType& size);

		FrameBuffer(GpuFrameBufferType type, const GpuFrameBufferSizeType& size);
		~FrameBuffer();

        void on_bind() const;

    private:
        GpuId id;
		boost::shared_ptr<Texture> color_texture;
		boost::shared_ptr<Texture> depth_texture;
		boost::shared_ptr<Texture> depth_stencil_texture;
        GpuFrameBufferSizeType size;
        GpuFrameBufferType type;

		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer& operator=(const FrameBuffer&) = delete;
    };
}
