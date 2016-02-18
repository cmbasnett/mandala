//glm
#include "glm\ext.hpp"

//naga
#include "camera_component.hpp"
#include "platform.hpp"
#include "gpu.hpp"
#include "collision.hpp"
#include "game_object.hpp"

namespace naga
{
    const char* camera_component::component_name = "CameraComponent";

    camera_params camera_component::get_params(const gpu_viewport_type& viewport) const
    {
        const auto aspect = static_cast<f32>(viewport.width) / glm::max(static_cast<f32>(viewport.height), 1.0f);
        const auto& pose = get_owner()->pose;

        camera_params params;
        params.location = pose.location;

        switch (projection_type)
        {
        case projection_type_e::ORTHOGRAPHIC:
            params.projection_matrix = glm::ortho(viewport.x, viewport.x + viewport.width, viewport.y + viewport.height, viewport.y);
            break;
        case projection_type_e::PERSPECTIVE:
            params.projection_matrix = glm::perspective(fov, aspect, near, far);
            break;
        }

        const auto forward = vec3(0, 0, 1) * glm::mat3_cast(pose.rotation);
        const auto left = glm::normalize(glm::cross(vec3(0, 1, 0), forward));
        auto up = glm::normalize(glm::cross(forward, left));

        //rotate up matrix along forward axis
        up = glm::rotate(glm::angleAxis(roll, forward), up);

        params.frustum.set(pose.location, left, up, forward, fov, near, far, aspect);
        params.view_matrix = glm::lookAt(pose.location, pose.location + forward, up);

        return params;
    }
    
    line3 camera_component::get_ray(const vec2_f64& screen_location) const
    {
        vec4 viewport = gpu.viewports.top();

        line3 ray;
        ray.start = glm::unProject(vec3(screen_location, 0), view_matrix, projection_matrix, viewport);
        ray.end = glm::unProject(vec3(screen_location, 1), view_matrix, projection_matrix, viewport);

        return ray;
    }
}
