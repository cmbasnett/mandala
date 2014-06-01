#pragma once

//mandala
#include "camera.hpp"
#include "input_event.hpp"

#define ARCBALL_CAMERA_DEFAULT_PITCH_MAX			(0.0f)
#define ARCBALL_CAMERA_DEFAULT_PITCH_MIN			(60.0f)
#define ARCBALL_CAMERA_DEFAULT_YAW					(45.0f)
#define ARCBALL_CAMERA_DEFAULT_DISTANCE_MIN			(8.0f)
#define ARCBALL_CAMERA_DEFAULT_DISTANCE_MAX			(32.0f)
#define ARCBALL_CAMERA_DEFAULT_DISTANCE				(16.0f)
#define ARCBALL_CAMERA_FOV_MIN						(70.0f)
#define ARCBALL_CAMERA_FOV_MAX						(40.0f)

namespace mandala
{
	struct arcball_camera_t : camera_t
	{
		virtual void tick(float32_t dt);
		void on_input_event(input_event_t& input_event);

		float32_t fov_min = ARCBALL_CAMERA_FOV_MIN;
		float32_t fov_max = ARCBALL_CAMERA_FOV_MAX;
		float32_t pitch_max = ARCBALL_CAMERA_DEFAULT_PITCH_MAX;
		float32_t pitch_min = ARCBALL_CAMERA_DEFAULT_PITCH_MIN;
		float32_t pitch = ARCBALL_CAMERA_DEFAULT_PITCH_MIN;
		float32_t pitch_target = ARCBALL_CAMERA_DEFAULT_PITCH_MIN;
		float32_t yaw = ARCBALL_CAMERA_DEFAULT_YAW;
		float32_t yaw_target = ARCBALL_CAMERA_DEFAULT_YAW;
		float32_t distance_min = ARCBALL_CAMERA_DEFAULT_DISTANCE_MIN;
		float32_t distance_max = ARCBALL_CAMERA_DEFAULT_DISTANCE_MAX;
		float32_t distance = ARCBALL_CAMERA_DEFAULT_DISTANCE;
		float32_t distance_target = ARCBALL_CAMERA_DEFAULT_DISTANCE;
		vec3_t location_target;
        vec3_t target_target;
		bool is_panning = false;
	};
};