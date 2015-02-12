#pragma once

//mandala
#include "types.hpp"
#include "line.hpp"
#include "frustum.hpp"

namespace mandala
{
	struct camera_t
	{
		enum class projection_type_e
		{
			orthographic,
			perspective
		};

		float32_t near = 0.5f;
		float32_t far = 1024.0f;
		float32_t aspect = 0.0f;
		float32_t fov = 90.0f;
		float32_t roll = 0.0f;
        vec3_t location;
        vec3_t target;
        projection_type_e projection_type = projection_type_e::perspective;

		virtual void tick(float32_t dt);

		line3_t get_ray(vec2_f64_t screen_location) const;

        const mat4_t& get_view_matrix() const { return view_matrix; }
        const mat4_t& get_projection_matrix() const { return projection_matrix; }
        const frustum_t& get_frustum() const { return frustum; }

    private:
        mat4_t view_matrix;
        mat4_t projection_matrix;
        frustum_t frustum;
	};
}
