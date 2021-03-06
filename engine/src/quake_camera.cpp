////std
//#include <iostream>
//
////naga
//#include "quake_camera.hpp"
//#include "input_event.hpp"
//#include "glm.hpp"
//
////glm
//#include <glm\gtx\norm.hpp>
//#include <glm\gtc\matrix_access.hpp>
//
//#define QUAKE_CAMERA_SMOOTHING_CONSTANT            (60.0f)
//
//namespace naga
//{
//    void quake_camera::on_tick(f32 dt)
//    {
//        const auto rotation_smoothing_value = glm::min(dt * (QUAKE_CAMERA_SMOOTHING_CONSTANT * smoothing_strength), 1.0f);
//        const auto roll_smoothing_value = glm::min(dt * (QUAKE_CAMERA_SMOOTHING_CONSTANT * 0.25f), 1.0f);
//        const auto movement_smoothing_value = glm::min(dt * (QUAKE_CAMERA_SMOOTHING_CONSTANT * 0.25f), 1.0f);
//
//        pitch_target += pitch_speed * dt;
//        yaw_target += yaw_speed * dt;
//        roll_target += roll_speed * dt;
//
//        pitch_target = glm::clamp(pitch_target, pitch_min, pitch_max);
//
//        pitch += (pitch_target - pitch) * rotation_smoothing_value;
//        yaw += (yaw_target - yaw) * rotation_smoothing_value;
//        roll += (roll_target - roll) * roll_smoothing_value;
//
//        pose.rotation = glm::angleAxis(-roll, vec3(0, 0, 1)) * glm::angleAxis(pitch, vec3(1, 0, 0)) * glm::angleAxis(yaw, vec3(0, 1, 0));
//
//        //temporary local velocity target variable so we don't modify the original
//        auto local_velocity_target_tick = local_velocity_target;
//
//        if (glm::length(local_velocity_target_tick) > 1.0f)
//        {
//            local_velocity_target_tick = glm::normalize(local_velocity_target);
//        }
//
//        local_velocity += (local_velocity_target_tick - local_velocity) * movement_smoothing_value;
//
//        velocity = glm::inverse(pose.rotation) * (local_velocity * speed_max);
//
//        pose.location += velocity * dt;
//
//        camera::on_tick(dt);
//    }
//
//    bool quake_camera::on_input_event(input_event_t& input_event)
//    {
//        switch (input_event.device_type)
//        {
//        case input_event_t::device_type_e::KEYBOARD:
//            {
//                switch (input_event.keyboard.key)
//                {
//                case input_event_t::keyboard_t::key_e::W:
//                    {
//                        switch(input_event.keyboard.type)
//                        {
//                        case input_event_t::keyboard_t::type_e::KEY_PRESS:
//                            {
//                                local_velocity_target.z = 1.0f;
//                                return true;
//                            }
//                            break;
//                        case input_event_t::keyboard_t::type_e::KEY_RELEASE:
//                            {
//                                local_velocity_target.z = 0.0f;
//                                return true;
//                            }
//                            break;
//                        }
//                    }
//                    break;
//                case input_event_t::keyboard_t::key_e::A:
//                    {
//                        switch(input_event.keyboard.type)
//                        {
//                        case input_event_t::keyboard_t::type_e::KEY_PRESS:
//                            {
//                                local_velocity_target.x = 1.0f;
//                                return true;
//                            }
//                            break;
//                        case input_event_t::keyboard_t::type_e::KEY_RELEASE:
//                            {
//                                local_velocity_target.x = 0.0f;
//                                return true;
//                            }
//                            break;
//                        }
//                    }
//                    break;
//                case input_event_t::keyboard_t::key_e::S:
//                    {
//                        switch(input_event.keyboard.type)
//                        {
//                        case input_event_t::keyboard_t::type_e::KEY_PRESS:
//                            {
//                                local_velocity_target.z = -1.0f;
//
//                                return true;
//                            }
//                            break;
//                        case input_event_t::keyboard_t::type_e::KEY_RELEASE:
//                            {
//                                local_velocity_target.z = 0.0f;
//                                return true;
//                            }
//                            break;
//                        }
//                    }
//                    break;
//                case input_event_t::keyboard_t::key_e::D:
//                    {
//                        switch(input_event.keyboard.type)
//                        {
//                        case input_event_t::keyboard_t::type_e::KEY_PRESS:
//                            {
//                                local_velocity_target.x = -1.0f;
//                                return true;
//                            }
//                            break;
//                        case input_event_t::keyboard_t::type_e::KEY_RELEASE:
//                            {
//                                local_velocity_target.x = 0.0f;
//                                return true;
//                            }
//                            break;
//                        }
//                    }
//                    break;
//                case input_event_t::keyboard_t::key_e::R:
//                    switch (input_event.keyboard.type)
//                    {
//                    case input_event_t::keyboard_t::type_e::KEY_PRESS:
//                        debug_is_rolling = true;
//                        return true;
//                    case input_event_t::keyboard_t::type_e::KEY_RELEASE:
//                        debug_is_rolling = false;
//                        return true;
//                    }
//                default:
//                    break;
//                }
//            }
//            break;
//        case input_event_t::device_type_e::TOUCH:
//            {
//                switch (input_event.touch.type)
//                {
//                case input_event_t::touch_t::type_e::MOVE:
//                    {
//                        if (debug_is_rolling)
//                        {
//                            roll_target += static_cast<f32>(input_event.touch.location_delta.x) * sensitivity;
//                        }
//                        else
//                        {
//                            pitch_target += static_cast<f32>(input_event.touch.location_delta.y) * sensitivity;
//                            yaw_target += static_cast<f32>(input_event.touch.location_delta.x) * sensitivity;
//                        }
//
//                        return true;
//                    }
//                    break;
//                default:
//                    break;
//                }
//            }
//            break;
//        case input_event_t::device_type_e::GAMEPAD:
//        {
//            switch (input_event.gamepad.type)
//            {
//            case input_event_t::gamepad_t::type_e::AXIS_MOVE:
//                {
//                    auto axis_value = glm::percent_rank(glm::abs(input_event.gamepad.axis_value), 0.125f, 1.0f);
//                    axis_value = glm::clamp(1.0f * -glm::log(-axis_value + 1), 0.0f, 1.0f);
//                    axis_value *= glm::sign(input_event.gamepad.axis_value);
//
//                    if (input_event.gamepad.axis_index == 0)
//                    {
//                        local_velocity_target.x = -axis_value;
//                        return true;
//                    }
//                    else if (input_event.gamepad.axis_index == 1)
//                    {
//                        local_velocity_target.z = axis_value;
//                        return true;
//                    }
//                    if (input_event.gamepad.axis_index == 4)
//                    {
//                        yaw_speed = axis_value * 240;
//                        return true;
//                    }
//                    else if (input_event.gamepad.axis_index == 3)
//                    {
//                        pitch_speed = -axis_value * 240;
//                        return true;
//                    }
//                }
//                    break;
//            }
//        }
//            break;
//        default:
//            break;
//        }
//
//        return false;
//    }
//}
