#pragma once

//std
#include <typeindex>
#include <map>

//boost
#include <boost\shared_ptr.hpp>
#include <boost\make_shared.hpp>

//mandala
#include "gpu_program.hpp"

namespace mandala
{
    struct gpu_program_mgr_t
    {
        template<typename T = std::enable_if<is_gpu_program<T>::value, T>::type>
        boost::shared_ptr<T> make()
        {
            static const std::type_index TYPE_INDEX = typeid(T);

            const auto gpu_program = boost::make_shared<T>();

            gpu_programs.insert(std::make_pair(TYPE_INDEX, gpu_program));

            return gpu_program;
        }

        template<typename T = std::enable_if<is_gpu_program<T>::value, T>::type>
        boost::shared_ptr<T> get()
        {
            static const std::type_index TYPE_INDEX = typeid(T);

            const auto gpu_programs_itr = gpu_programs.find(TYPE_INDEX);

            if (gpu_programs_itr == gpu_programs.end())
            {
                throw std::out_of_range("gpu program does not exist");
            }

            return boost::static_pointer_cast<T, gpu_program_t>(gpu_programs_itr->second);
        }

        void purge();

    private:
        std::map<std::type_index, boost::shared_ptr<gpu_program_t>> gpu_programs;
    };

    extern gpu_program_mgr_t gpu_programs;
}
