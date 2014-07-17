#pragma once

//mandala
#include "resource.hpp"

namespace mandala
{
	struct shader_t;

	struct gpu_program_t : resource_t
	{
		gpu_program_t(std::istream& istream);
		virtual ~gpu_program_t();

		uint32_t id = 0;

	private:
		gpu_program_t(gpu_program_t&) = delete;
		gpu_program_t& operator=(gpu_program_t&) = delete;
	};
};
