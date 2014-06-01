//mandala
#include "quake_camera.hpp"
#include "input_event.hpp"
#include "glm.hpp"

//glm
#include <glm\gtx\norm.hpp>
#include <glm\gtc\matrix_access.hpp>

#define QUAKE_CAMERA_SMOOTHING_CONSTANT			(60.0f)

namespace mandala
{
	quake_camera_t::quake_camera_t() :
		pitch_max(89.9f),
		pitch_min(-89.9f),
		pitch(0),
		yaw(0),
		roll(0),
		pitch_target(pitch),
		yaw_target(yaw),
		smoothing_strength(0.8f),
		speed_max(25),
		pitch_speed(0),
		yaw_speed(0)
	{
	}

	void quake_camera_t::tick(float32_t dt)
	{
		const float32_t smoothing_value = glm::min(dt * (QUAKE_CAMERA_SMOOTHING_CONSTANT * smoothing_strength), 1.0f);

		pitch_target += pitch_speed * dt;
		yaw_target += yaw_speed * dt;

		pitch_target = glm::clamp(pitch_target, pitch_min, pitch_max);

		pitch += (pitch_target - pitch) * smoothing_value;
		yaw += (yaw_target - yaw) * smoothing_value;

		auto rotation_quaternion = glm::angleAxis(pitch, vec3_t(1, 0, 0)) * glm::angleAxis(yaw, vec3_t(0, 1, 0));
		auto rotation_matrix = glm::mat3_cast(glm::normalize(rotation_quaternion));
		auto forward = glm::row(rotation_matrix, 2);

		if (glm::length(local_velocity_target) > 1.0f)
		{
			local_velocity_target = glm::normalize(local_velocity_target);
		}

		local_velocity += (local_velocity_target - local_velocity) * smoothing_value;

		velocity = glm::inverse(rotation_matrix) * (local_velocity * speed_max);
		position += velocity * dt;

		target = position + forward;

		camera_t::tick(dt);
	}

	void quake_camera_t::on_input_event(input_event_t& input_event)
	{
		switch (input_event.device_type)
		{
		case input_event_t::device_type_t::keyboard:
			{
				switch (input_event.keyboard.key)
				{
				case input_event_t::keyboard_t::key_t::w:
					{
						switch(input_event.keyboard.type)
						{
						case input_event_t::keyboard_t::type_t::key_press:
							{
								local_velocity_target.z = 1.0f;
								input_event.is_consumed = true;
							}
							break;
						case input_event_t::keyboard_t::type_t::key_release:
							{
								local_velocity_target.z = 0.0f;
								input_event.is_consumed = true;
							}
							break;
						}
					}
					break;
				case input_event_t::keyboard_t::key_t::a:
					{
						switch(input_event.keyboard.type)
						{
						case input_event_t::keyboard_t::type_t::key_press:
							{
								local_velocity_target.x = 1.0f;
								input_event.is_consumed = true;
							}
							break;
						case input_event_t::keyboard_t::type_t::key_release:
							{
								local_velocity_target.x = 0.0f;
								input_event.is_consumed = true;
							}
							break;
						}
					}
					break;
				case input_event_t::keyboard_t::key_t::s:
					{
						switch(input_event.keyboard.type)
						{
						case input_event_t::keyboard_t::type_t::key_press:
							{
								local_velocity_target.z = -1.0f;
								input_event.is_consumed = true;
							}
							break;
						case input_event_t::keyboard_t::type_t::key_release:
							{
								local_velocity_target.z = 0.0f;
								input_event.is_consumed = true;
							}
							break;
						}
					}
					break;
				case input_event_t::keyboard_t::key_t::d:
					{
						switch(input_event.keyboard.type)
						{
						case input_event_t::keyboard_t::type_t::key_press:
							{
								local_velocity_target.x = -1.0f;
								input_event.is_consumed = true;
							}
							break;
						case input_event_t::keyboard_t::type_t::key_release:
							{
								local_velocity_target.x = 0.0f;
								input_event.is_consumed = true;
							}
							break;
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		case input_event_t::device_type_t::touch:
			{
				switch (input_event.touch.type)
				{
				case input_event_t::touch_t::type_t::move:
					{
						pitch_target -= (float32_t)input_event.touch.position_delta.y * sensitivity;
						yaw_target += (float32_t)input_event.touch.position_delta.x * sensitivity;

						input_event.is_consumed = true;
					}
					break;
				default:
					break;
				}
			}
			break;
		case input_event_t::device_type_t::gamepad:
		{
			switch (input_event.gamepad.type)
			{
				case input_event_t::gamepad_t::type_t::axis_move:
				{
					auto axis_value = glm::percent_rank(glm::abs(input_event.gamepad.axis_value), 0.125f, 1.0f);
					axis_value = glm::clamp(1.0f * -glm::log(-axis_value + 1), 0.0f, 1.0f);
					axis_value *= glm::sign(input_event.gamepad.axis_value);

					if (input_event.gamepad.axis_index == 0)
					{
						local_velocity_target.x = -axis_value;
						input_event.is_consumed = true;
					}
					else if (input_event.gamepad.axis_index == 1)
					{
						local_velocity_target.z = axis_value;
						input_event.is_consumed = true;
					}
					if (input_event.gamepad.axis_index == 4)
					{
						yaw_speed = axis_value * 240;
						input_event.is_consumed = true;
					}
					else if (input_event.gamepad.axis_index == 3)
					{
						pitch_speed = -axis_value * 240;
						input_event.is_consumed = true;
					}
				}
					break;
			}
		}
			break;
		default:
			break;
		}
	}
};