#pragma once

//mandala
#include "types.hpp"
#include "camera.hpp"

namespace mandala
{
    struct input_event_t;

    struct quake_camera_t : camera_t
    {
        float32_t pitch_max = 89.9f;
        float32_t pitch_min = -89.9f;
        float32_t pitch = 0;
        float32_t yaw = 0;
        float32_t pitch_target = 0;
        float32_t yaw_target = 0;
        float32_t roll_target = 0;
        float32_t smoothing_strength = 0.5f;
        vec3_t local_velocity_target;
        vec3_t local_velocity;
        vec3_t velocity;
        float32_t pitch_speed = 0;
        float32_t yaw_speed = 0;
        float32_t roll_speed = 0;
        float32_t speed_max = 512;
        float32_t sensitivity = 0.1f;    //TODO: read sensitivity from configuration file?
        bool debug_is_rolling = false;
        
        virtual void on_tick(float32_t dt) override;

        bool on_input_event(input_event_t& input_event) override;
    };
}
