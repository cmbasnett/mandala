#pragma once

//mandala
#include "types.hpp"
#include "camera.hpp"

namespace mandala
{
	struct input_event_t;

	struct quake_camera_t : camera_t
	{
		float32_t pitch_max;
		float32_t pitch_min;
		float32_t pitch;
		float32_t yaw;
		float32_t roll;
		float32_t pitch_target;
		float32_t yaw_target;
		float32_t roll_target;
		float32_t smoothing_strength;
		vec3_t local_velocity_target;
		vec3_t local_velocity;
        vec3_t velocity;
		float32_t pitch_speed;
		float32_t yaw_speed;
		float32_t speed_max;
		float32_t sensitivity = 0.1f;	//TODO: read sensitivity from configuration file?

		quake_camera_t();
		
		virtual void tick(float32_t dt);

		void on_input_event(input_event_t& input_event);
	};
}
