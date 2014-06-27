//gl
#include <GL\glew.h>

//mandala
#include "frame_buffer.hpp"
#include "texture.hpp"

namespace mandala
{
    frame_buffer_t::frame_buffer_t(uint32_t width, uint32_t height)
    {
        glGenFramebuffers(1, &id);
        glBindFramebuffer(GL_FRAMEBUFFER, id);

        color_texture = std::make_shared<texture_t>(color_type_t::rgb, width, height);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture->id, 0);

        glGenRenderbuffers(1, &depth_stencil_render_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_render_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_render_buffer);
    }

    frame_buffer_t::~frame_buffer_t()
    {
        glDeleteRenderbuffers(1, &depth_stencil_render_buffer); glCheckError();
        glDeleteFramebuffers(1, &id); glCheckError();
    }
};
