// naga
#include "types.hpp"
#include "input_device.hpp"
#include "enum.hpp"

namespace naga
{
	DEFINE_ENUM_WITH_STRING_CONVERSIONS(InputActionType, (PRESS)(RELEASE)(REPEAT)(MOVE)(CHARACTER)(SCROLL))

	struct InputEventType
	{
		InputDeviceType device;
		InputActionType action;
		union
		{
			i32 key;
			i32 button;
			i32 axis;
			wchar_t character;
		};
		u8 flags;
	};
}