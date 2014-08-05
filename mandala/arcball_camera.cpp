//glm
#include <glm\gtc\matrix_access.hpp>
#include <glm\ext.hpp>

//mandala
#include "arcball_camera.hpp"

#define ARCBALL_CAMERA_SMOOTHING_CONSTANT			(60.0f)
#define ARCBALL_CAMERA_DEFAULT_SMOOTHING_STRENGTH	(0.25f)

namespace mandala
{
	void arcball_camera_t::tick(float32_t dt)
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

		auto q = quat_t(vec3_t(0, glm::radians(yaw), glm::radians(pitch)));
		auto m = glm::mat3_cast(glm::normalize(q));
		auto forward = glm::row(m, 0);

        //position
		position = target - (forward * distance);

		camera_t::tick(dt);
	}

	void arcball_camera_t::on_input_event(input_event_t& input_event)
	{
		switch (input_event.device_type)
		{
		case input_event_t::device_type_e::touch:
			{
				switch (input_event.touch.type)
				{
				case input_event_t::touch_t::type_e::button_press:
					{
						switch (input_event.touch.button)
						{
						case input_event_t::touch_t::button_e::middle:
							{
								is_panning = true;

								input_event.is_consumed = true;
							}
							break;
						default:
							break;
						}
					}
					break;
				case input_event_t::touch_t::type_e::button_release:
					{
						switch (input_event.touch.button)
						{
						case input_event_t::touch_t::button_e::middle:
							{
								is_panning = false;

								input_event.is_consumed = true;
							}
							break;
						default:
							break;
						}
					}
					break;
				case input_event_t::touch_t::type_e::move:
					{
						if(is_panning)
						{
							auto forward = target - position;
							forward.y = 0;
							forward = glm::normalize(forward);

							auto left = glm::cross(vec3_t(0, 1, 0), forward);

							forward *= static_cast<float32_t>(input_event.touch.position_delta.y) * 0.1f;
							left *= static_cast<float32_t>(input_event.touch.position_delta.x) * 0.1f;

							auto target_delta = forward + left;

							target_target += target_delta;

							input_event.is_consumed = true;
						}
					}
					break;
				case input_event_t::touch_t::type_e::scroll:
					{
						distance_target += static_cast<float32_t>(input_event.touch.position.y) * 10.0f;

						input_event.is_consumed = true;
					}
					break;
				default:
					break;
				}
			}
			break;
        case input_event_t::device_type_e::keyboard:
        {
            if (input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_press)
            {
				if (input_event.keyboard.key == input_event_t::keyboard_t::key_e::q)
                {
					yaw_target += 90;
                }
				if (input_event.keyboard.key == input_event_t::keyboard_t::key_e::e)
                {
					yaw_target -= 90;
                }
            }
        }
            break;
		default:
			break;
		}
	}
}
