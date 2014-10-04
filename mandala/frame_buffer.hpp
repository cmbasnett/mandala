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
        typedef uint8_t type_flags_type;

        enum : type_flags_type
        {
            type_flag_color = (1 << 0),
            type_flag_depth = (1 << 1),
            type_flag_stencil = (1 << 2)
        };

        enum class type_e : type_flags_type
        {
            color = (type_flag_color),
            color_depth = (type_flag_color | type_flag_depth),
            color_depth_stencil = (type_flag_color | type_flag_depth | type_flag_stencil),
            depth = (type_flag_depth),
            depth_stencil = (type_flag_depth | type_flag_stencil)
        };

        id_type id;
        std::shared_ptr<texture_t> color_texture;
        std::shared_ptr<texture_t> depth_texture;
        std::shared_ptr<texture_t> depth_stencil_texture;
        size_type size;
        type_e type;

        frame_buffer_t(type_e type, const size_type& size);
        frame_buffer_t(type_e type, uint32_t width, uint32_t height);
        ~frame_buffer_t();

        void on_bind() const;

    private:
        frame_buffer_t(const frame_buffer_t&) = delete;
        frame_buffer_t& operator=(const frame_buffer_t&) = delete;
    };
}
