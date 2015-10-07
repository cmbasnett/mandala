#pragma once

//mandala
#include "types.hpp"
#include "camera.hpp"

namespace mandala
{
    struct input_event_t;

    struct quake_camera : camera
    {
        f32 pitch_max = 89.9f;
        f32 pitch_min = -89.9f;
        f32 pitch = 0;
        f32 yaw = 0;
        f32 pitch_target = 0;
        f32 yaw_target = 0;
        f32 roll_target = 0;
        f32 smoothing_strength = 0.5f;
        vec3 local_velocity_target;
        vec3 local_velocity;
        vec3 velocity;
        f32 pitch_speed = 0;
        f32 yaw_speed = 0;
        f32 roll_speed = 0;
        f32 speed_max = 512;
        f32 sensitivity = 0.1f;    //TODO: read sensitivity from configuration file?
        bool debug_is_rolling = false;
        
        virtual void on_tick(f32 dt) override;

        bool on_input_event(input_event_t& input_event) override;
    };
}
