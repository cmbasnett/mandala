//gl
#include <GL\glew.h>

//mandala
#include "frame_buffer.hpp"
#include "texture.hpp"
#include "render_buffer.hpp"

namespace mandala
{
    frame_buffer_t::frame_buffer_t()
    {
        glGenFramebuffers(1, &id); glCheckError();
    }

    frame_buffer_t::~frame_buffer_t()
    {
        glDeleteFramebuffers(1, &id); glCheckError();
    }

    void frame_buffer_t::bind(mode_t mode) const
    {
        glBindFramebuffer(static_cast<GLenum>(mode), id); glCheckError();
        
        auto frame_buffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (frame_buffer_status != GL_FRAMEBUFFER_COMPLETE)
        {
            throw std::exception();
        }
    }

    void frame_buffer_t::unbind(mode_t mode) const
    {
        glBindFramebuffer(static_cast<GLenum>(mode), 0); glCheckError();
    }

    void frame_buffer_t::attach(mode_t mode, attachment_type_t attachment_type, std::shared_ptr<texture_t> texture)
    {
        glBindFramebuffer(static_cast<GLenum>(mode), id); glCheckError();
        glFramebufferTexture2D(static_cast<GLenum>(mode), static_cast<GLenum>(attachment_type), GL_TEXTURE_2D, texture->id, 0); glCheckError();
        glBindFramebuffer(static_cast<GLenum>(mode), 0); glCheckError();
    }

    void frame_buffer_t::attach(mode_t mode, attachment_type_t attachment_type, std::shared_ptr<render_buffer_t> render_buffer)
    {
        glBindFramebuffer(static_cast<GLenum>(mode), id); glCheckError();
        glFramebufferRenderbuffer(static_cast<GLenum>(mode), static_cast<GLenum>(attachment_type), GL_RENDERBUFFER, render_buffer->id); glCheckError();
        glBindFramebuffer(static_cast<GLenum>(mode), 0); glCheckError();
    }

    void frame_buffer_t::detach(mode_t mode, attachment_type_t attachment_type)
    {
        glBindFramebuffer(static_cast<GLenum>(mode), id); glCheckError();
        glFramebufferTexture(static_cast<GLenum>(mode), static_cast<GLenum>(attachment_type), 0, 0); glCheckError();
        glBindFramebuffer(static_cast<GLenum>(mode), 0); glCheckError();
    }
};
