//mandala
#include "opengl.hpp"
#include "frame_buffer.hpp"
#include "texture.hpp"
#include "gpu.hpp"

namespace mandala
{
    frame_buffer_t::frame_buffer_t(const size_type& size) :
        frame_buffer_t(size.x, size.y)
    {
    }

    frame_buffer_t::frame_buffer_t(uint32_t width, uint32_t height) :
        size(width, height)
    {
        glGenFramebuffers(1, &id);
        glBindFramebuffer(GL_FRAMEBUFFER, id);

        color_texture = std::make_shared<texture_t>(color_type_e::rgb, width, height);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture->id, 0);

        glGenRenderbuffers(1, &depth_stencil_render_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_render_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_render_buffer);

		//restore previously bound frame buffer
		auto frame_buffer = gpu.frame_buffers.top();

		if (frame_buffer)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer->lock()->id);
		}
    }

    frame_buffer_t::~frame_buffer_t()
    {
        glDeleteRenderbuffers(1, &depth_stencil_render_buffer); glCheckError();
        glDeleteFramebuffers(1, &id); glCheckError();
    }
}
