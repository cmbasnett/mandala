#pragma once

//std
#include <memory>
#include <map>

//mandala
#include "types.hpp"

//gl
#include <GL\glew.h>

namespace mandala
{
    struct texture_t;
    struct render_buffer_t;

    struct frame_buffer_t
    {
        enum class attachment_type_t : GLenum
        {
            color0 = GL_COLOR_ATTACHMENT0,
            color1 = GL_COLOR_ATTACHMENT1,
            color2 = GL_COLOR_ATTACHMENT2,
            color3 = GL_COLOR_ATTACHMENT3,
            color4 = GL_COLOR_ATTACHMENT4,
            color5 = GL_COLOR_ATTACHMENT5,
            color6 = GL_COLOR_ATTACHMENT6,
            color7 = GL_COLOR_ATTACHMENT7,
            color8 = GL_COLOR_ATTACHMENT8,
            color9 = GL_COLOR_ATTACHMENT9,
            color10 = GL_COLOR_ATTACHMENT10,
            color11 = GL_COLOR_ATTACHMENT11,
            color12 = GL_COLOR_ATTACHMENT12,
            color13 = GL_COLOR_ATTACHMENT13,
            color14 = GL_COLOR_ATTACHMENT14,
            color15 = GL_COLOR_ATTACHMENT15,
            depth = GL_DEPTH_ATTACHMENT,
            stencil = GL_STENCIL_ATTACHMENT,
            depth_and_stencil = GL_DEPTH_STENCIL_ATTACHMENT
        };

        enum class mode_t : GLenum
        {
            read = GL_READ_FRAMEBUFFER,
            draw = GL_DRAW_FRAMEBUFFER,
            read_draw = GL_FRAMEBUFFER
        };

        frame_buffer_t();
        ~frame_buffer_t();

        void bind(mode_t mode) const;
        void unbind(mode_t mode) const;
        void attach(mode_t mode, attachment_type_t attachment_type, std::shared_ptr<texture_t> texture);
        void attach(mode_t mode, attachment_type_t attachment_type, std::shared_ptr<render_buffer_t> render_buffer);
        void detach(mode_t mode, attachment_type_t attachment_type);

        uint32_t id = 0;

    private:
        frame_buffer_t(const frame_buffer_t&) = delete;
        frame_buffer_t& operator=(const frame_buffer_t&) = delete;
    };
};


