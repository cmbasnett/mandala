#pragma once

// std
#include <string>

// naga
#include "platform_defs.hpp"
#include "types.hpp"
#include "input_device.hpp"
#include "input_event_type.hpp"

namespace naga
{
	struct InputEvent
	{
		struct Mouse
		{
			size_t id;
			f32 x, y, dx, dy;

			inline vec2 location() const { return vec2(x, y); }
			inline vec2 location_delta() const { return vec2(dx, dy); }
		};

		struct Gamepad
		{
			u8 index;
			f32 value;
			f32 delta;
		};

		InputEvent() = default;
		InputEvent(const std::string& string);

		size_t id = 0;
		InputEventType type;

		union
		{
			Mouse mouse;
			Gamepad gamepad;
		};

	private:
		friend std::ostream& operator<<(std::ostream& ostream, const InputEvent& input_event);
	};
}