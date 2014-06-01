#pragma once

//mandala
#include "types.hpp"
#include "line.hpp"
#include "frustum.hpp"

namespace mandala
{
	struct camera_t
	{
		enum class projection_type_t
		{
			orthographic,
			perspective
		};

		vec4_t viewport;
		mat4_t view;
		mat4_t projection;
		float32_t near = 0.1f;
		float32_t far = 1024.0f;
		float32_t aspect = 0.0f;
		float32_t fov = 45.0f;
		vec3_t target;
		vec3_t position;
		frustum_t frustum;
		projection_type_t projection_type = projection_type_t::perspective;

		virtual void tick(float32_t dt);

		line3_t get_ray(vec2_f64_t screen_location) const;

	protected:
		camera_t();
	};
};