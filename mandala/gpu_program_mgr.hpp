#pragma once

//mandala
#include <map>
#include <memory>

namespace mandala
{
	struct gpu_program_t;

	struct gpu_program_mgr_t
	{
		std::map<std::string, std::shared_ptr<gpu_program_t>> gpu_programs;
	};
}
