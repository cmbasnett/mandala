#pragma once

//boost
#include <boost\shared_ptr.hpp>

//mandala
#include "gpu_program.hpp"
#include "bsp.hpp"

namespace mandala
{
    struct bsp_gpu_program_t : gpu_program_t
    {
        typedef bsp_t::vertex_type vertex_type;

        static std::string vertex_shader_source;
        static std::string fragment_shader_source;

        bsp_gpu_program_t();

        void on_bind() override;
        void on_unbind() override;

    private:
        gpu_location_t location_location = -1;
        gpu_location_t diffuse_texcoord_location = -1;
        gpu_location_t lightmap_texcoord_location = -1;
    };
}