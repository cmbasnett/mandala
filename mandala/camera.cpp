//glm
#include "glm\ext.hpp"

//mandala
#include "mandala.h"
#include "camera.h"

namespace mandala
{
	camera_t::camera_t()
	{
	}

	void camera_t::tick(float32_t dt)
	{
		vec4_t viewport;
		
		//TODO: get rid of GL call here
		glGetFloatv(GL_VIEWPORT, glm::value_ptr(viewport));

		aspect = viewport.z / viewport.w;

		auto forward = glm::normalize(target - position);
        auto left = glm::normalize(glm::cross(vec3_t(0, 1, 0), forward));
        auto up = glm::normalize(glm::cross(forward, left));

		switch (projection_type)
		{
			case projection_type_t::orthographic:
			{
				projection = glm::ortho(0.0f, viewport.x, viewport.y, 0.0f);
				break;
			}
			case projection_type_t::perspective:
			{
				projection = glm::perspective(fov, aspect, near, far);
				break;
			}
		}

		view = glm::lookAt(position, target, up);

		frustum = view * projection;
	}

	line3_t camera_t::get_ray(vec2_f64_t screen_location) const
	{
		//todo: swap screen_location.y before we get here?
		screen_location.y = viewport.w - screen_location.y;

		line3_t ray;
		ray.start = glm::unProject(vec3_t(screen_location, 0), view, projection, viewport);
		ray.end = glm::unProject(vec3_t(screen_location, 1), view, projection, viewport);

		return ray;
	}
};