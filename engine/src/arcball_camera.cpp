//glm
#include <glm\gtc\matrix_access.hpp>
#include <glm\ext.hpp>

//mandala
#include "arcball_camera.hpp"

#define ARCBALL_CAMERA_SMOOTHING_CONSTANT            (60.0f)
#define ARCBALL_CAMERA_DEFAULT_SMOOTHING_STRENGTH    (0.25f)

namespace mandala
{
    void arcball_camera_t::on_tick(f32 dt)
    {
        const auto smoothing_value = glm::min(dt * (ARCBALL_CAMERA_SMOOTHING_CONSTANT * ARCBALL_CAMERA_DEFAULT_SMOOTHING_STRENGTH), 1.0f);

        //distance
        distance_target = glm::clamp(distance_target, distance_min, distance_max);
        distance += (distance_target - distance) * smoothing_value;

        auto distance_ratio = (distance_max - distance - distance_min) / (distance_max - distance_min);
        
        //fov
        fov = fov_min + ((fov_max - fov_min) * distance_ratio);

        //pitch
        pitch = pitch_min + ((pitch_max - pitch_min) * distance_ratio);

        //yaw
        yaw += (yaw_target - yaw) * smoothing_value;

        //target
        target += (target_target - target) * smoothing_value;

        pose.rotation = glm::angleAxis(-roll, vec3(0, 0, 1)) * glm::angleAxis(pitch, vec3(1, 0, 0)) * glm::angleAxis(yaw, vec3(0, 1, 0));
        mat3 asd = glm::mat3_cast(pose.rotation);
        const auto forward = vec3(0, 0, 1) * asd;

        //location
        pose.location = target - (forward * distance);

        camera_t::on_tick(dt);
    }

    bool arcball_camera_t::on_input_event(input_event_t& input_event)
    {
        auto forward = glm::mul(vec3(0, 0, 1), glm::mat3_cast(pose.rotation));

        switch (input_event.device_type)
        {
        case input_event_t::device_type_e::TOUCH:
            {
                switch (input_event.touch.type)
                {
                case input_event_t::touch_t::type_e::PRESS:
                    {
                        switch (input_event.touch.button)
                        {
                        case input_event_t::touch_t::button_e::MIDDLE:
                            {
                                is_panning = true;
                                return true;
                            }
                            break;
                        default:
                            break;
                        }
                    }
                    break;
                case input_event_t::touch_t::type_e::RELEASE:
                    {
                        switch (input_event.touch.button)
                        {
                        case input_event_t::touch_t::button_e::MIDDLE:
                            {
                                is_panning = false;
                                return true;
                            }
                            break;
                        default:
                            break;
                        }
                    }
                    break;
                case input_event_t::touch_t::type_e::MOVE:
                    {
                        if(is_panning)
                        {
                            forward.y = 0;
                            forward = glm::normalize(forward);

                            auto left = glm::cross(vec3(0, 1, 0), forward);

                            forward *= static_cast<f32>(-input_event.touch.location_delta.y) * 0.1f;
                            left *= static_cast<f32>(input_event.touch.location_delta.x) * 0.1f;

                            auto target_delta = forward + left;

                            target_target += target_delta;

                            return true;
                        }
                    }
                    break;
                case input_event_t::touch_t::type_e::SCROLL:
                    {
                        distance_target += static_cast<f32>(input_event.touch.location_delta.y) * 32.0f;

                        return true;
                    }
                    break;
                default:
                    break;
                }
            }
            break;
        case input_event_t::device_type_e::KEYBOARD:
        {
            if (input_event.keyboard.type == input_event_t::keyboard_t::type_e::KEY_PRESS)
            {
                if (input_event.keyboard.key == input_event_t::keyboard_t::key_e::Q)
                {
                    yaw_target += 90;

                    return true;
                }
                if (input_event.keyboard.key == input_event_t::keyboard_t::key_e::E)
                {
                    yaw_target -= 90;

                    return true;
                }
            }
        }
            break;
        default:
            break;
        }

        return false;
    }
}
