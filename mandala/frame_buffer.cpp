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
		color_texture = std::make_shared<texture_t>(color_type_e::rgb, width, height);
        depth_stencil_render_buffer = std::make_shared<texture_t>(color_type_e::depth24_stencil8, width, height);

		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture->id, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_stencil_render_buffer->id, 0);

		//restore previously bound frame buffer
		auto frame_buffer = gpu.frame_buffers.top();

		if (frame_buffer)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer->lock()->id);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

    frame_buffer_t::~frame_buffer_t()
    {
        glDeleteFramebuffers(1, &id); glCheckError();
    }
}
