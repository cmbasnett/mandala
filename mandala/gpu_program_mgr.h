#pragma once

//std
#include <typeindex>
#include <map>
#include <memory>

//mandala
#include "gpu_program.hpp"

namespace mandala
{
	struct gpu_program_mgr_t
	{
		template<typename T = std::enable_if<std::is_base_of<gpu_program_t, T>::value>>
		void make()
		{
			static const std::type_index type_index = typeid(T);

			auto gpu_program = std::make_shared<T>();

			gpu_programs.insert(std::make_pair(type_index, gpu_program));
		}

		template<typename T = std::enable_if<std::is_base_of<gpu_program_t, T>::value>>
		void get()
		{
			static const std::type_index type_index = typeid(T);

			auto gpu_programs_itr = gpu_programs.find(type_index);

			if (gpu_programs_itr == gpu_programs.end())
			{
				throw std::out_of_range("");
			}

			return gpu_programs_itr->second;
		}

		void purge()
		{
			gpu_programs.clear();
		}

		std::map<std::type_index, std::shared_ptr<gpu_program_t>> gpu_programs;
	};
}
