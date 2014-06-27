//mandala
#include "graphics_mgr.hpp"
#include "gpu_program.hpp"
#include "frame_buffer.hpp"
#include "texture.hpp"

//glew
#include <GL\glew.h>

namespace mandala
{
	gpu_mgr_t gpu;

	std::shared_ptr<gpu_program_t> gpu_mgr_t::get_gpu_program() const
	{
		if (gpu_programs.size() == 0)
		{
			return std::shared_ptr<gpu_program_t>();
		}

		return gpu_programs.top();
	}

	void gpu_mgr_t::push_gpu_program(std::shared_ptr<gpu_program_t> gpu_program)
	{
		if (gpu_programs.size() == 0 || gpu_program != gpu_programs.top())
		{
			glUseProgram(gpu_program->id);
		}

		gpu_programs.push(gpu_program);
	}

	void gpu_mgr_t::pop_gpu_program()
	{
		if (gpu_programs.size() == 0)
		{
			throw std::exception();
		}

		gpu_programs.pop();

		auto gpu_program = get_gpu_program();

		if (gpu_program == nullptr)
		{
			glUseProgram(0);
		}
		else
		{
			glUseProgram(gpu_program->id);
		}
	}

	std::shared_ptr<frame_buffer_t> gpu_mgr_t::get_frame_buffer() const
	{
		if (gpu_programs.size() == 0)
		{
			return std::shared_ptr<frame_buffer_t>();
		}

		return frame_buffers.top();
	}

	void gpu_mgr_t::push_frame_buffer(std::shared_ptr<frame_buffer_t> frame_buffer)
	{
		if (gpu_programs.size() == 0 || frame_buffer != frame_buffers.top())
		{
			glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer->id);
		}

		frame_buffers.push(frame_buffer);
	}

	void gpu_mgr_t::pop_frame_buffer()
	{
		if (frame_buffers.size() == 0)
		{
			throw std::exception();
		}

		frame_buffers.pop();

		auto frame_buffer = get_frame_buffer();

		if (frame_buffer == nullptr)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer->id);
		}
	}


	std::shared_ptr<texture_t> gpu_mgr_t::get_texture(uint8_t index)
	{
		return textures[index];
	}

	void gpu_mgr_t::bind_texture(std::shared_ptr<texture_t> texture, uint8_t index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, texture != nullptr ? texture->id : 0);

		textures[index] = texture;
	}

	void gpu_mgr_t::unbind_texture(uint8_t index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, 0);

		textures[index] = nullptr;
	}
};
