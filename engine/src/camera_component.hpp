#pragma once

//naga
#include "types.hpp"
#include "line.hpp"
#include "frustum.hpp"
#include "game_component.hpp"
#include "camera_params.hpp"
#include "gpu_defs.hpp"
#include "game_component_defs.hpp"

namespace naga
{
    struct camera_component : game_component
    {
        static const char* component_name;

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

		line3 get_ray(const gpu_viewport_type& viewport, const vec2& screen_location) const;
        camera_params get_params(const gpu_viewport_type& viewport) const;

        camera_component() { }
        ~camera_component() { }

    private:
        mat4 view_matrix;
        mat4 projection_matrix;
        frustum frustum;
    };
}
