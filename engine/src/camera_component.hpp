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
    struct CameraComponent : GameComponent
	{
		static const char* component_name;

        enum class ProjectionType
        {
            ORTHOGRAPHIC,
            PERSPECTIVE
        };

        f32 near = 0.25f;
        f32 far = 8192.0f;
        f32 fov = 90.0f;
        f32 roll = 0.0f;
		ProjectionType projection_type = ProjectionType::PERSPECTIVE;

		Line3 get_ray(const GpuViewportType& viewport, const vec2& screen_location) const;
        CameraParameters get_parameters(const GpuViewportType& viewport) const;

		CameraComponent() { }
		~CameraComponent() { }

    private:
        mat4 view_matrix;
        mat4 projection_matrix;
		Frustum frustum;
    };
}
