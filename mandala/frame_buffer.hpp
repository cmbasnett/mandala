#pragma once

//std
#include <memory>
#include <map>

//mandala
#include "types.hpp"

namespace mandala
{
    struct texture_t;

    struct frame_buffer_t
    {
        typedef uint32_t id_type;
        typedef vec2_u32_t size_type;

        frame_buffer_t(const size_type& size);
        frame_buffer_t(uint32_t width, uint32_t height);
        ~frame_buffer_t();

        id_type id;
        std::shared_ptr<texture_t> color_texture;
        uint32_t depth_stencil_render_buffer;
        size_type size;

    private:
        frame_buffer_t(const frame_buffer_t&) = delete;
        frame_buffer_t& operator=(const frame_buffer_t&) = delete;
    };
}
