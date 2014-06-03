//mandala
#include "mandala.hpp"
#include "render_buffer.hpp"

//gl
#include <GL\glew.h>

namespace mandala
{
    render_buffer_t::scope_bind_t::scope_bind_t(const render_buffer_t& render_buffer) :
        render_buffer(render_buffer)
    {
        render_buffer.bind();
    }

    render_buffer_t::scope_bind_t::~scope_bind_t()
    {
        render_buffer.unbind();
    }

    render_buffer_t::render_buffer_t(type_t type, uint32_t width, uint32_t height)
    {
        glGenRenderbuffers(1, &id); glCheckError();

        int32_t internal_format = 0;

        switch (type)
        {
        case type_t::depth:
            internal_format = GL_DEPTH_COMPONENT;
            break;
        case type_t::stencil:
            internal_format = GL_STENCIL_INDEX;
            break;
        }

        auto scope_bind = scope_bind_t(*this);

        glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height); glCheckError();
    }

    render_buffer_t::~render_buffer_t()
    {
        glDeleteRenderbuffers(1, &id); glCheckError();
    }

    void render_buffer_t::bind() const
    {
        glBindRenderbuffer(GL_RENDERBUFFER, id); glCheckError();
    }

    void render_buffer_t::unbind() const
    {
        glBindRenderbuffer(GL_RENDERBUFFER, 0); glCheckError();
    }

    int32_t render_buffer_t::get_width() const
    {
        auto scope_bind = scope_bind_t(*this);

        int32_t width = 0;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width); glCheckError();
        return width;
    }

    int32_t render_buffer_t::get_height() const
    {
        auto scope_bind = scope_bind_t(*this);

        int32_t height = 0;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height); glCheckError();
        return height;
    }

    int32_t render_buffer_t::get_internal_format() const
    {
        auto scope_bind = scope_bind_t(*this);

        int32_t internal_format = 0;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &internal_format); glCheckError();
        return internal_format;
    }

    int32_t render_buffer_t::get_red_size() const
    {
        auto scope_bind = scope_bind_t(*this);

        int32_t red_size = 0;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_RED_SIZE, &red_size); glCheckError();
        return red_size;
    }

    int32_t render_buffer_t::get_green_size() const
    {
        auto scope_bind = scope_bind_t(*this);

        int32_t green_size = 0;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_GREEN_SIZE, &green_size); glCheckError();
        return green_size;
    }

    int32_t render_buffer_t::get_blue_size() const
    {
        auto scope_bind = scope_bind_t(*this);

        int32_t blue_size = 0;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_BLUE_SIZE, &blue_size); glCheckError();
        return blue_size;
    }

    int32_t render_buffer_t::get_alpha_size() const
    {
        auto scope_bind = scope_bind_t(*this);

        int32_t alpha_size = 0;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_ALPHA_SIZE, &alpha_size); glCheckError();
        return alpha_size;
    }

    int32_t render_buffer_t::get_depth_size() const
    {
        auto scope_bind = scope_bind_t(*this);

        int32_t depth_size = 0;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_DEPTH_SIZE, &depth_size); glCheckError();
        return depth_size;
    }

    int32_t render_buffer_t::get_stencil_size() const
    {
        auto scope_bind = scope_bind_t(*this);

        int32_t stencil_size = 0;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_STENCIL_SIZE, &stencil_size); glCheckError();
        return stencil_size;
    }
}
