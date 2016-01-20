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
    struct texture;

    struct frame_buffer
    {
        size_t get_id() const { return id; }
        const boost::shared_ptr<texture>& get_color_texture() const { return color_texture; }
        const boost::shared_ptr<texture>& get_depth_texture() const { return depth_texture; }
        const boost::shared_ptr<texture>& get_depth_stencil_texture() const { return depth_stencil_texture; }
        const gpu_frame_buffer_size_type& get_size() const { return size; }
        gpu_frame_buffer_type get_type() const { return type; }

        void set_size(const gpu_frame_buffer_size_type& size);

        frame_buffer(gpu_frame_buffer_type type, const gpu_frame_buffer_size_type& size);
        ~frame_buffer();

        void on_bind() const;

    private:
        gpu_id id;
        boost::shared_ptr<texture> color_texture;
        boost::shared_ptr<texture> depth_texture;
        boost::shared_ptr<texture> depth_stencil_texture;
        gpu_frame_buffer_size_type size;
        gpu_frame_buffer_type type;

        frame_buffer(const frame_buffer&) = delete;
        frame_buffer& operator=(const frame_buffer&) = delete;
    };
}
