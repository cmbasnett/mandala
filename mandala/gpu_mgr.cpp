//mandala
#include "gpu_mgr.hpp"
#include "gpu_program.hpp"
#include "frame_buffer.hpp"
#include "texture.hpp"
#include "gpu_buffer.hpp"

//glew
#include <GL\glew.h>

namespace mandala
{
	gpu_mgr_t gpu;

    inline GLenum get_target(gpu_mgr_t::buffer_target_e target)
    {
        switch (target)
        {
        case gpu_mgr_t::buffer_target_e::array:
            return GL_ARRAY_BUFFER;
        case gpu_mgr_t::buffer_target_e::atomic_counter:
            return GL_ATOMIC_COUNTER_BUFFER;
        case gpu_mgr_t::buffer_target_e::copy_read:
            return GL_COPY_READ_BUFFER;
        case gpu_mgr_t::buffer_target_e::copy_write:
            return GL_COPY_WRITE_BUFFER;
        case gpu_mgr_t::buffer_target_e::dispatch_indirect:
            return GL_DISPATCH_INDIRECT_BUFFER;
        case gpu_mgr_t::buffer_target_e::draw_indrect:
            return GL_DRAW_INDIRECT_BUFFER;
        case gpu_mgr_t::buffer_target_e::element_array:
            return GL_ELEMENT_ARRAY_BUFFER;
        case gpu_mgr_t::buffer_target_e::pixel_pack:
            return GL_PIXEL_PACK_BUFFER;
        case gpu_mgr_t::buffer_target_e::pixel_unpack:
            return GL_PIXEL_UNPACK_BUFFER;
        case gpu_mgr_t::buffer_target_e::query:
            return GL_QUERY_BUFFER;
        case gpu_mgr_t::buffer_target_e::shader_storage:
            return GL_SHADER_STORAGE_BUFFER;
        default:
            throw std::invalid_argument("");
        }
    }

    inline GLenum get_usage(gpu_mgr_t::buffer_usage_e usage)
    {
        switch (usage)
        {
        case gpu_mgr_t::buffer_usage_e::stream_draw:
            return GL_STREAM_DRAW;
        case gpu_mgr_t::buffer_usage_e::stream_read:
            return GL_STREAM_READ;
        case gpu_mgr_t::buffer_usage_e::stream_copy:
            return GL_STREAM_COPY;
        case gpu_mgr_t::buffer_usage_e::static_draw:
            return GL_STATIC_DRAW;
        case gpu_mgr_t::buffer_usage_e::static_read:
            return GL_STATIC_READ;
        case gpu_mgr_t::buffer_usage_e::static_copy:
            return GL_STATIC_COPY;
        case gpu_mgr_t::buffer_usage_e::dynamic_draw:
            return GL_DYNAMIC_DRAW;
        case gpu_mgr_t::buffer_usage_e::dynamic_read:
            return GL_DYNAMIC_READ;
        case gpu_mgr_t::buffer_usage_e::dynamic_copy:
            return GL_DYNAMIC_COPY;
        default:
            throw std::invalid_argument("");
        }
    }

    gpu_mgr_t::program_mgr_t::weak_type gpu_mgr_t::program_mgr_t::get() const
	{
        if (programs.empty())
		{
            throw new std::out_of_range("");
		}

        return programs.top();
	}

    void gpu_mgr_t::program_mgr_t::push(const gpu_mgr_t::program_mgr_t::shared_type& program)
    {
        if (programs.empty() || program != programs.top())
		{
            glUseProgram(program->id); glCheckError();
		}

        programs.push(program);
	}

    gpu_mgr_t::program_mgr_t::weak_type gpu_mgr_t::program_mgr_t::pop()
	{
        if (programs.size() == 0)
		{
			throw std::exception();
		}

		programs.pop();

        if (programs.empty())
		{
            glUseProgram(0); glCheckError();

            return weak_type();
		}
		else
		{
            glUseProgram(programs.top()->id); glCheckError();
            
            return programs.top();
		}
	}

    gpu_mgr_t::frame_buffer_mgr_t::weak_type gpu_mgr_t::frame_buffer_mgr_t::get() const
    {
        if (frame_buffers.empty())
        {
            throw new std::out_of_range("");
        }

        return frame_buffers.top();
    }

    void gpu_mgr_t::frame_buffer_mgr_t::push(const gpu_mgr_t::frame_buffer_mgr_t::shared_type& frame_buffer)
    {
        if (frame_buffers.size() == 0 || frame_buffer != frame_buffers.top())
        {
            glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer->id); glCheckError();
        }

        frame_buffers.push(frame_buffer);
    }

    gpu_mgr_t::frame_buffer_mgr_t::weak_type gpu_mgr_t::frame_buffer_mgr_t::pop()
    {
        if (frame_buffers.size() == 0)
        {
            throw std::exception();
        }

        frame_buffers.pop();

        if (frame_buffers.empty())
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0); glCheckError();

            return weak_type();
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, frame_buffers.top()->id); glCheckError();

            return frame_buffers.top();
        }
    }

    gpu_mgr_t::texture_mgr_t::weak_type gpu_mgr_t::texture_mgr_t::get(index_type index) const
    {
        if (index < 0 || index >= texture_count)
        {
            throw std::out_of_range("");
        }

        return textures[index];
    }

    gpu_mgr_t::texture_mgr_t::weak_type gpu_mgr_t::texture_mgr_t::bind(index_type index, const shared_type& texture)
    {
        const auto& previous_texture = textures[index];

        if (previous_texture == texture)
        {
            return previous_texture;
        }

        glActiveTexture(GL_TEXTURE0 + index); glCheckError();
        glBindTexture(GL_TEXTURE_2D, texture != nullptr ? texture->id : 0); glCheckError();

        textures[index] = texture;

        return previous_texture;
    }

    gpu_mgr_t::texture_mgr_t::weak_type gpu_mgr_t::texture_mgr_t::unbind(index_type index)
    {
        const auto& previous_texture = textures[index];

        if (previous_texture == nullptr)
        {
            return previous_texture;
        }

        glActiveTexture(GL_TEXTURE0 + index); glCheckError();
        glBindTexture(GL_TEXTURE_2D, 0); glCheckError();

        textures[index] = nullptr;

        return previous_texture;
    }

    gpu_mgr_t::viewport_mgr_t::viewport_type gpu_mgr_t::viewport_mgr_t::get() const
    {
        if (viewports.empty())
        {
            return viewport_type();
        }

        return viewports.top();
    }

    void gpu_mgr_t::viewport_mgr_t::push(const viewport_type& viewport)
    {
        viewports.push(viewport);

        glViewport(viewport.x, viewport.y, viewport.width, viewport.height); glCheckError();
    }

    gpu_mgr_t::viewport_mgr_t::viewport_type gpu_mgr_t::viewport_mgr_t::pop()
    {
        if (viewports.empty())
        {
            throw std::exception();
        }

        auto previous_viewport = viewports.top();

        glViewport(previous_viewport.x, previous_viewport.y, previous_viewport.width, previous_viewport.height); glCheckError();

        viewports.pop();

        return previous_viewport;
    }

    void gpu_mgr_t::buffer_mgr_t::push(buffer_target_e target, buffer_type buffer)
    {
        if (target_buffers.find(target) == target_buffers.end())
        {
            target_buffers.emplace(std::make_pair(target, std::stack<buffer_type>()));
        }

        target_buffers[target].push(buffer);

        glBindBuffer(get_target(target), buffer->id); glCheckError();
    }

    gpu_mgr_t::buffer_mgr_t::buffer_type gpu_mgr_t::buffer_mgr_t::pop(buffer_target_e target)
    {
        if (target_buffers.find(target) == target_buffers.end())
        {
            throw std::out_of_range("");
        }

        auto& buffers = target_buffers[target];

        if (buffers.empty())
        {
            throw std::out_of_range("");
        }

        buffers.pop();

        glBindBuffer(get_target(target), buffers.empty() ? 0 : buffers.top()->id); glCheckError();

        return buffers.empty() ? buffer_type() : buffers.top();
    }

    void gpu_mgr_t::buffer_mgr_t::data(buffer_target_e target, void* data, size_t size, buffer_usage_e usage)
    {
        glBufferData(get_target(target), size, data, get_usage(usage)); glCheckError();
    }
};
