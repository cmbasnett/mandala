// naga
#include "types.hpp"
#include "input_device.hpp"
#include "input_event_flags.hpp"
#include "enum.hpp"
#include "keyboard.hpp"

namespace naga
{
	DEFINE_ENUM_WITH_STRING_CONVERSIONS(InputActionType, (PRESS)(RELEASE)(REPEAT)(MOVE)(CHARACTER)(SCROLL))

	// TODO: we would like some key aliases for look-up purposes

	struct InputEventType
	{
		InputDeviceType device;
		InputActionType action;
		union
		{
			Key key;
			i32 button;
			i32 axis;
			i32 direction;
			wchar_t character;
		};
		u8 flags;

		friend std::ostream& operator<<(std::ostream& ostream, const InputEventType& input_event_type);
		friend std::istream& operator>>(std::istream& istream, const InputEventType& input_event_type);
	};
}