#pragma once

//std
#include <memory>

//mandala
#include "types.hpp"
#include "gpu.hpp"

namespace mandala
{
	struct gpu_program_t : std::enable_shared_from_this<gpu_program_t>
	{
		virtual ~gpu_program_t();

		virtual void on_bind() = 0;
		virtual void on_unbind() = 0;

		gpu_t::program_id_type id = 0;

	protected:
		gpu_program_t(const std::string& vertex_shader_source, const std::string& fragment_shader_source);

	private:
		gpu_program_t(const gpu_program_t&) = delete;
		gpu_program_t& operator=(const gpu_program_t&) = delete;
	};
};
