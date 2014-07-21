//mandala
#include "opengl.hpp"
#include "gpu.hpp"
#include "gpu_program.hpp"
#include "frame_buffer.hpp"
#include "texture.hpp"
#include "gpu_buffer.hpp"

namespace mandala
{
	gpu_t gpu;

    inline GLenum get_buffer_target(gpu_t::buffer_target_e buffer_target)
    {
        switch (buffer_target)
        {
        case gpu_t::buffer_target_e::array:
            return GL_ARRAY_BUFFER;
        case gpu_t::buffer_target_e::atomic_counter:
            return GL_ATOMIC_COUNTER_BUFFER;
        case gpu_t::buffer_target_e::copy_read:
            return GL_COPY_READ_BUFFER;
        case gpu_t::buffer_target_e::copy_write:
            return GL_COPY_WRITE_BUFFER;
        case gpu_t::buffer_target_e::dispatch_indirect:
            return GL_DISPATCH_INDIRECT_BUFFER;
        case gpu_t::buffer_target_e::draw_indrect:
            return GL_DRAW_INDIRECT_BUFFER;
        case gpu_t::buffer_target_e::element_array:
            return GL_ELEMENT_ARRAY_BUFFER;
        case gpu_t::buffer_target_e::pixel_pack:
            return GL_PIXEL_PACK_BUFFER;
        case gpu_t::buffer_target_e::pixel_unpack:
            return GL_PIXEL_UNPACK_BUFFER;
        case gpu_t::buffer_target_e::query:
            return GL_QUERY_BUFFER;
        case gpu_t::buffer_target_e::shader_storage:
            return GL_SHADER_STORAGE_BUFFER;
        default:
            throw std::invalid_argument("");
        }
    }

    inline GLenum get_buffer_usage(gpu_t::buffer_usage_e buffer_usage)
    {
        switch (buffer_usage)
        {
        case gpu_t::buffer_usage_e::stream_draw:
            return GL_STREAM_DRAW;
        case gpu_t::buffer_usage_e::stream_read:
            return GL_STREAM_READ;
        case gpu_t::buffer_usage_e::stream_copy:
            return GL_STREAM_COPY;
        case gpu_t::buffer_usage_e::static_draw:
            return GL_STATIC_DRAW;
        case gpu_t::buffer_usage_e::static_read:
            return GL_STATIC_READ;
        case gpu_t::buffer_usage_e::static_copy:
            return GL_STATIC_COPY;
        case gpu_t::buffer_usage_e::dynamic_draw:
            return GL_DYNAMIC_DRAW;
        case gpu_t::buffer_usage_e::dynamic_read:
            return GL_DYNAMIC_READ;
        case gpu_t::buffer_usage_e::dynamic_copy:
            return GL_DYNAMIC_COPY;
        default:
            throw std::invalid_argument("");
        }
    }

    inline GLenum get_primitive_type(gpu_t::primitive_type_e primitive_type)
    {
        switch (primitive_type)
        {
        case gpu_t::primitive_type_e::points:
            return GL_POINTS;
        case gpu_t::primitive_type_e::lines:
            return GL_LINES;
        case gpu_t::primitive_type_e::line_strip:
            return GL_LINE_STRIP;
        case gpu_t::primitive_type_e::triangles:
            return GL_TRIANGLES;
        case gpu_t::primitive_type_e::triangle_strip:
            return GL_TRIANGLE_STRIP;
        case gpu_t::primitive_type_e::triangle_fan:
            return GL_TRIANGLE_FAN;
        case gpu_t::primitive_type_e::quads:
            return GL_QUADS;
        case gpu_t::primitive_type_e::quad_strip:
            return GL_QUAD_STRIP;
        case gpu_t::primitive_type_e::polygon:
            return GL_POLYGON;
        default:
            throw std::invalid_argument("");
        }
    }

    inline GLenum get_index_type(gpu_t::index_type_e index_type)
    {
        switch (index_type)
        {
        case gpu_t::index_type_e::unsigned_byte:
            return GL_UNSIGNED_BYTE;
        case gpu_t::index_type_e::unsigned_short:
            return GL_UNSIGNED_SHORT;
        case gpu_t::index_type_e::unsigned_int:
            return GL_UNSIGNED_INT;
        default:
            throw std::invalid_argument("");
        }
    }

    gpu_t::program_mgr_t::weak_type gpu_t::program_mgr_t::get() const
	{
        if (programs.empty())
		{
            throw new std::out_of_range("");
		}

        return programs.top();
	}

    void gpu_t::program_mgr_t::push(const gpu_t::program_mgr_t::shared_type& program)
    {
        if (programs.empty() || program != programs.top())
		{
            glUseProgram(program->id); glCheckError();
		}

        programs.push(program);
	}

    gpu_t::program_mgr_t::weak_type gpu_t::program_mgr_t::pop()
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

    gpu_t::frame_buffer_mgr_t::weak_type gpu_t::frame_buffer_mgr_t::get() const
    {
        if (frame_buffers.empty())
        {
            throw new std::out_of_range("");
        }

        return frame_buffers.top();
    }

    void gpu_t::frame_buffer_mgr_t::push(const gpu_t::frame_buffer_mgr_t::shared_type& frame_buffer)
    {
        if (frame_buffers.size() == 0 || frame_buffer != frame_buffers.top())
        {
            glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer->id); glCheckError();
        }

        frame_buffers.push(frame_buffer);
    }

    gpu_t::frame_buffer_mgr_t::weak_type gpu_t::frame_buffer_mgr_t::pop()
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

    gpu_t::texture_mgr_t::weak_type gpu_t::texture_mgr_t::get(index_type index) const
    {
        if (index < 0 || index >= texture_count)
        {
            throw std::out_of_range("");
        }

        return textures[index];
    }

    gpu_t::texture_mgr_t::weak_type gpu_t::texture_mgr_t::bind(index_type index, const shared_type& texture)
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

    gpu_t::texture_mgr_t::weak_type gpu_t::texture_mgr_t::unbind(index_type index)
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

    gpu_t::viewport_mgr_t::viewport_type gpu_t::viewport_mgr_t::peek() const
    {
        if (viewports.empty())
        {
            return viewport_type();
        }

        return viewports.top();
    }

    void gpu_t::viewport_mgr_t::push(const viewport_type& viewport)
    {
        viewports.push(viewport);

        glViewport(viewport.x, viewport.y, viewport.width, viewport.height); glCheckError();
    }

    gpu_t::viewport_mgr_t::viewport_type gpu_t::viewport_mgr_t::pop()
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

    void gpu_t::buffer_mgr_t::push(buffer_target_e target, buffer_type buffer)
    {
        if (target_buffers.find(target) == target_buffers.end())
        {
            target_buffers.emplace(std::make_pair(target, std::stack<buffer_type>()));
        }

        target_buffers[target].push(buffer);

        glBindBuffer(get_buffer_target(target), buffer->id); glCheckError();
    }

    gpu_t::buffer_mgr_t::buffer_type gpu_t::buffer_mgr_t::pop(buffer_target_e target)
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

        glBindBuffer(get_buffer_target(target), buffers.empty() ? 0 : buffers.top()->id); glCheckError();

        return buffers.empty() ? buffer_type() : buffers.top();
    }

    void gpu_t::buffer_mgr_t::data(buffer_target_e target, void* data, size_t size, buffer_usage_e usage)
    {
        glBufferData(get_buffer_target(target), size, data, get_buffer_usage(usage)); glCheckError();
    }

    void gpu_t::draw_elements(primitive_type_e primitive_type, size_t count, index_type_e index_type, size_t offset)
    {
        glDrawElements(get_primitive_type(primitive_type), count, get_index_type(index_type), reinterpret_cast<GLvoid*>(offset)); glCheckError();
    }
};
