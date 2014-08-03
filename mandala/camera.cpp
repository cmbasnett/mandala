//glm
#include "glm\ext.hpp"

//mandala
#include "camera.hpp"
#include "platform.hpp"

namespace mandala
{
	camera_t::camera_t()
	{
	}

	void camera_t::tick(float32_t dt)
	{
		vec4_t viewport;
		
		//TODO: have current viewport set in some sort of graphics manager
		auto window_size = static_cast<vec2_t>(platform.get_window_size());
		aspect = window_size.x / window_size.y;

		auto forward = glm::normalize(target - position);
        auto left = glm::normalize(glm::cross(vec3_t(0, 1, 0), forward));
        auto up = glm::normalize(glm::cross(forward, left));

		switch (projection_type)
		{
			case projection_type_e::orthographic:
			{
				projection_matrix = glm::ortho(0.0f, viewport.x, viewport.y, 0.0f);
				break;
			}
			case projection_type_e::perspective:
			{
                projection_matrix = glm::perspective(fov, aspect, near, far);
				break;
			}
		}

		view_matrix = glm::lookAt(position, target, up);

        frustum = view_matrix * projection_matrix;
	}

	line3_t camera_t::get_ray(vec2_f64_t screen_location) const
	{
		//todo: swap screen_location.y before we get here?
		screen_location.y = viewport.w - screen_location.y;

		line3_t ray;
        ray.start = glm::unProject(vec3_t(screen_location, 0), view_matrix, projection_matrix, viewport);
        ray.end = glm::unProject(vec3_t(screen_location, 1), view_matrix, projection_matrix, viewport);

		return ray;
	}
};
