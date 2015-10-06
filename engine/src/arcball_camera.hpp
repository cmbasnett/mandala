#pragma once

//mandala
#include "camera.hpp"
#include "input_event.hpp"

namespace mandala
{
    struct arcball_camera_t : camera_t
    {
        virtual void on_tick(f32 dt) override;
        bool on_input_event(input_event_t& input_event);

        f32 fov_min = 70.0f;
        f32 fov_max = 40.0f;
        f32 pitch_max = -30.0f;
        f32 pitch_min = -80.0f;
        f32 pitch = 60.0f;
        f32 pitch_target = 60.0f;
        f32 yaw = 45.0f;
        f32 yaw_target = 45.0f;
        f32 roll_target = 0.0f;
        f32 distance_min = 64.0f;
        f32 distance_max = 1024.0f;
        f32 distance = 64.0f;
        f32 distance_target = 64.0f;
        vec3 location_target;
        vec3 target;
        vec3 target_target;
        bool is_panning = false;
    };
}
