#pragma once

//std
#include <memory>
#include <map>

//mandala
#include "types.hpp"
#include "gpu_defs.hpp"

namespace mandala
{
    struct texture_t;

    struct frame_buffer_t
    {
		gpu_id_t id;
        std::shared_ptr<texture_t> color_texture;
        std::shared_ptr<texture_t> depth_texture;
        std::shared_ptr<texture_t> depth_stencil_texture;
        gpu_frame_buffer_size_type size;
		gpu_frame_buffer_type_e type;

        frame_buffer_t(gpu_frame_buffer_type_e type, const gpu_frame_buffer_size_type& size);
        frame_buffer_t(gpu_frame_buffer_type_e type, gpu_frame_buffer_size_type::value_type width, gpu_frame_buffer_size_type::value_type height);
        ~frame_buffer_t();

        void on_bind() const;

    private:
        frame_buffer_t(const frame_buffer_t&) = delete;
        frame_buffer_t& operator=(const frame_buffer_t&) = delete;
    };
}
