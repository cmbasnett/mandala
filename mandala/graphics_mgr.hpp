#pragma once

//std
#include <stack>
#include <memory>
#include <array>

namespace mandala
{
	struct gpu_program_t;
	struct frame_buffer_t;
	struct texture_t;

	struct gpu_mgr_t
	{
		//gpu programs
		std::shared_ptr<gpu_program_t> get_gpu_program() const;
		void push_gpu_program(std::shared_ptr<gpu_program_t> gpu_program);
		void pop_gpu_program();

		//frame buffers
		std::shared_ptr<frame_buffer_t> get_frame_buffer() const;
		void push_frame_buffer(std::shared_ptr<frame_buffer_t> frame_buffer);
		void pop_frame_buffer();

		//textures
		std::shared_ptr<texture_t> get_texture(uint8_t index);
		void bind_texture(std::shared_ptr<texture_t>, uint8_t index);
		void unbind_texture(uint8_t index);

	private:
		std::stack<std::shared_ptr<gpu_program_t>> gpu_programs;
		std::stack<std::shared_ptr<frame_buffer_t>> frame_buffers;
		std::array<std::shared_ptr<texture_t>, 32> textures;
	};

	extern gpu_mgr_t gpu;
};
