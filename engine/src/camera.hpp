#pragma once

//mandala
#include "types.hpp"
#include "line.hpp"
#include "frustum.hpp"
#include "actor.hpp"
#include "camera_params.hpp"
#include "gpu_defs.hpp"

namespace mandala
{
    struct camera_t : actor_t
    {
        enum class projection_type_e
        {
            ORTHOGRAPHIC,
            PERSPECTIVE
        };

        float32_t near = 0.25f;
        float32_t far = 8192.0f;
        float32_t fov = 90.0f;
        float32_t roll = 0.0f;
        vec3_t target;
        projection_type_e projection_type = projection_type_e::PERSPECTIVE;

        line3_t get_ray(const vec2_f64_t& screen_location) const;
        camera_params get_params(const gpu_viewport_type& viewport) const;

    private:
        mat4_t view_matrix;
        mat4_t projection_matrix;
        frustum_t frustum;
    };
}
