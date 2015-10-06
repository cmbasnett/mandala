//glm
#include "glm\ext.hpp"

//mandala
#include "camera.hpp"
#include "platform.hpp"
#include "gpu.hpp"
#include "collision.hpp"

namespace mandala
{
    camera_params camera_t::get_params(const gpu_viewport_type& viewport) const
    {
        const auto aspect = static_cast<float32_t>(viewport.width) / glm::max(static_cast<float32_t>(viewport.height), 1.0f);

        camera_params params;
        params.location = pose.location;

        switch (projection_type)
        {
        case projection_type_e::ORTHOGRAPHIC:
        {
            params.projection_matrix = glm::ortho(viewport.x, viewport.x + viewport.width, viewport.y + viewport.height, viewport.y);
        }
        case projection_type_e::PERSPECTIVE:
        {
            params.projection_matrix = glm::perspective(fov, aspect, near, far);
        }
        }

        const auto forward = vec3_t(0, 0, 1) * glm::mat3_cast(pose.rotation);
        const auto left = glm::normalize(glm::cross(vec3_t(0, 1, 0), forward));
        auto up = glm::normalize(glm::cross(forward, left));

        //rotate up matrix along forward axis
        up = glm::rotate(glm::angleAxis(roll, forward), up);

        params.frustum.set(pose.location, left, up, forward, fov, near, far, aspect);
        params.view_matrix = glm::lookAt(pose.location, pose.location + forward, up);

        return params;
    }
    
    line3_t camera_t::get_ray(const vec2_f64_t& screen_location) const
    {
        vec4_t viewport = gpu.viewports.top();

        line3_t ray;
        ray.start = glm::unProject(vec3_t(screen_location, 0), view_matrix, projection_matrix, viewport);
        ray.end = glm::unProject(vec3_t(screen_location, 1), view_matrix, projection_matrix, viewport);

        return ray;
    }
}
