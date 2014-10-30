#pragma once

//std
#include <memory>

//mandala
#include "gpu_program.hpp"

namespace mandala
{
    struct shadow_gpu_program_t : gpu_program_t
    {
        struct vertex_t
        {
            vec3_t position;
        };

        typedef vertex_t vertex_type;

        static std::string vertex_shader_source;
        static std::string fragment_shader_source;

        shadow_gpu_program_t();

        void on_bind() override;
        void on_unbind() override;

        void world_matrix(const mat4_t&) const;
        void view_projection_matrix(const mat4_t&) const;
        void texture_matrix(const mat4_t&) const;

    private:
        gpu_t::attribute_location_type position_location = -1;
        gpu_t::uniform_location_type world_matrix_location = -1;
        gpu_t::uniform_location_type view_projection_matrix_location = -1;
        gpu_t::uniform_location_type texture_matrix_location = -1;
    };
}