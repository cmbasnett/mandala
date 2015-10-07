#pragma once

//boost
#include <boost\enable_shared_from_this.hpp>

//mandala
#include "types.hpp"
#include "gpu.hpp"

namespace mandala
{
    struct gpu_program : boost::enable_shared_from_this<gpu_program>
    {
        virtual ~gpu_program();

        virtual void on_bind() = 0;
        virtual void on_unbind() = 0;

        gpu_id get_id() const { return id; }

    protected:
        gpu_program(const std::string& vertex_shader_source, const std::string& fragment_shader_source);

    private:
        gpu_id id;

        gpu_program(const gpu_program&) = delete;
        gpu_program& operator=(const gpu_program&) = delete;
    };

    template<typename T, typename Enable = void>
    struct is_gpu_program : std::false_type
    {
    };

    template<typename T>
    struct is_gpu_program<T, typename std::enable_if<std::is_base_of<gpu_program, T>::value>::type> : std::true_type
    {
    };
}