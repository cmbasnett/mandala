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

        f32 near = 0.25f;
        f32 far = 8192.0f;
        f32 fov = 90.0f;
        f32 roll = 0.0f;
        projection_type_e projection_type = projection_type_e::PERSPECTIVE;

        line3 get_ray(const vec2_f64& screen_location) const;
        camera_params get_params(const gpu_viewport_type& viewport) const;

    private:
        mat4 view_matrix;
        mat4 projection_matrix;
        frustum frustum;
    };
}
