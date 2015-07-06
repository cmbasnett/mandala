//glm
#include "glm\ext.hpp"

//mandala
#include "camera.hpp"
#include "platform.hpp"
#include "gpu.hpp"
#include "collision.hpp"

namespace mandala
{
	void camera_t::tick(float32_t dt)
	{
        auto& viewport = gpu.viewports.top();
		
		const auto window_size = static_cast<vec2_t>(platform.get_window_size());
        aspect = window_size.x / glm::max(window_size.y, 1.0f);

		const auto forward = glm::normalize(target - pose.location);
		const auto left = glm::normalize(glm::cross(vec3_t(0, 1, 0), forward));
        auto up = glm::normalize(glm::cross(forward, left));

		switch (projection_type)
		{
			case projection_type_e::ORTHOGRAPHIC:
			{
				projection_matrix = glm::ortho(viewport.x, viewport.x + viewport.width, viewport.y + viewport.height, viewport.y);
				break;
			}
			case projection_type_e::PERSPECTIVE:
			{
                projection_matrix = glm::perspective(fov, aspect, near, far);
				break;
			}
		}

		//rotate up matrix along forward axis
		up = glm::rotate(glm::angleAxis(roll, forward), up);

        view_matrix = glm::lookAt(pose.location, target, up);

        frustum.set(pose.location, left, up, forward, fov, near, far, aspect);
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