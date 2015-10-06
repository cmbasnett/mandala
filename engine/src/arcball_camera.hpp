#pragma once

//mandala
#include "camera.hpp"
#include "input_event.hpp"

namespace mandala
{
    struct arcball_camera_t : camera_t
    {
        virtual void on_tick(float32_t dt) override;
        bool on_input_event(input_event_t& input_event);

        float32_t fov_min = 70.0f;
        float32_t fov_max = 40.0f;
        float32_t pitch_max = -30.0f;
        float32_t pitch_min = -80.0f;
        float32_t pitch = 60.0f;
        float32_t pitch_target = 60.0f;
        float32_t yaw = 45.0f;
        float32_t yaw_target = 45.0f;
        float32_t roll_target = 0.0f;
        float32_t distance_min = 64.0f;
        float32_t distance_max = 1024.0f;
        float32_t distance = 64.0f;
        float32_t distance_target = 64.0f;
        vec3_t location_target;
        vec3_t target;
        vec3_t target_target;
        bool is_panning = false;
    };
}
