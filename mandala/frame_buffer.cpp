//mandala
#include "opengl.hpp"
#include "frame_buffer.hpp"
#include "texture.hpp"
#include "gpu.hpp"

namespace mandala
{
    frame_buffer_t::frame_buffer_t(type_e type, const size_type& size) :
        frame_buffer_t(type, size.x, size.y)
    {
	}

    frame_buffer_t::frame_buffer_t(type_e type, uint32_t width, uint32_t height) :
        type(type),
		size(width, height)
    {
        glGenFramebuffers(1, &id); glCheckError();
        glBindFramebuffer(GL_FRAMEBUFFER, id); glCheckError();

        auto type_flags = static_cast<type_flags_type>(type);

        //color
        if ((type_flags & type_flag_color) == type_flag_color)
        {
            color_texture = std::make_shared<texture_t>(color_type_e::rgb, width, height);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture->id, 0); glCheckError();
        }
        else
        {
            glDrawBuffer(GL_NONE); glCheckError();
            glReadBuffer(GL_NONE); glCheckError();
        }

        //depth & stencil
        if ((type_flags & (type_flag_depth | type_flag_stencil)) == (type_flag_depth | type_flag_stencil))
        {
            depth_stencil_texture = std::make_shared<texture_t>(color_type_e::depth_stencil, width, height);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_stencil_texture->id, 0); glCheckError();
        }
        //depth
        else if ((type_flags & type_flag_depth) == type_flag_depth)
        {
            depth_texture = std::make_shared<texture_t>(color_type_e::depth, width, height);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture->id, 0); glCheckError();
        }

		//restore previously bound frame buffer
        auto frame_buffer = gpu.frame_buffers.top();

        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer ? frame_buffer->lock()->id : 0); glCheckError();
	}

    frame_buffer_t::~frame_buffer_t()
    {
        glDeleteFramebuffers(1, &id); glCheckError();
    }

    void frame_buffer_t::on_bind() const
    {
        //NOTE: setting these flags on binding could have unforseen consequences
        auto gpu_color_state = gpu.color.get_state();
        auto gpu_depth_state = gpu.depth.get_state();

        if ((static_cast<type_flags_type>(type) & type_flag_color) == type_flag_color)
        {
            gpu_color_state.mask.r = true;
            gpu_color_state.mask.g = true;
            gpu_color_state.mask.b = true;
            gpu_color_state.mask.a = true;
        }
        else
        {
            gpu_color_state.mask.r = false;
            gpu_color_state.mask.g = false;
            gpu_color_state.mask.b = false;
            gpu_color_state.mask.a = false;
        }

        if ((static_cast<type_flags_type>(type) & type_flag_depth) == type_flag_depth)
        {
            gpu_depth_state.should_write_mask = true;
        }
        else
        {
            gpu_depth_state.should_write_mask = false;
        }

        gpu.color.push_state(gpu_color_state);
        gpu.depth.push_state(gpu_depth_state);

        //TODO: stencil mask
    }
}
