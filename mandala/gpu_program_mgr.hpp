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
		template<typename T = std::enable_if<is_gpu_program<T>::value, T>::type>
        std::shared_ptr<T> make()
		{
			static const std::type_index type_index = typeid(T);

            const auto gpu_program = std::make_shared<T>();

			gpu_programs.insert(std::make_pair(type_index, gpu_program));

            return gpu_program;
		}

        template<typename T = std::enable_if<is_gpu_program<T>::value, T>::type>
        std::shared_ptr<T> get()
		{
			static const std::type_index type_index = typeid(T);

			const auto gpu_programs_itr = gpu_programs.find(type_index);

			if (gpu_programs_itr == gpu_programs.end())
			{
                throw std::out_of_range("");
			}

            return std::static_pointer_cast<T, gpu_program_t>(gpu_programs_itr->second);
		}

        void purge();

	private:
		std::map<std::type_index, std::shared_ptr<gpu_program_t>> gpu_programs;
	};

	extern gpu_program_mgr_t gpu_programs;
}
