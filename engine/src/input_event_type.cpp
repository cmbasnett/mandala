#include "input_event_type.hpp"

namespace naga
{
	std::ostream& operator<<(std::ostream& ostream, const InputEventType& input_event_type)
	{
		switch (input_event_type.device)
		{
		case InputDeviceType::GAMEPAD:
			switch (input_event_type.action)
			{
			case InputActionType::PRESS:
			case InputActionType::RELEASE:
				ostream << "JOY " << input_event_type.button;
				break;
			case InputActionType::MOVE:
				ostream << "JOYAXIS " << input_event_type.axis;
				break;
			}
			break;
		case InputDeviceType::KEYBOARD:
			switch (input_event_type.action)
			{
			case InputActionType::PRESS:
			case InputActionType::RELEASE:
			case InputActionType::REPEAT:
				ostream << input_event_type.key;
				break;
			case InputActionType::CHARACTER:
				ostream << input_event_type.character;
				break;
			}
			break;
		case InputDeviceType::MOUSE:
			switch (input_event_type.action)
			{
			case InputActionType::SCROLL:
				ostream << (input_event_type.direction < 0 ? "MWHEELUP" : "MWHEELDN");
				break;
			case InputActionType::PRESS:
			case InputActionType::RELEASE:
				ostream << "MOUSE " << input_event_type.button;
				break;
			}
			break;
		}
		return ostream;
	}
	
	std::istream& operator>>(std::istream& istream, const InputEventType& input_event_type)
	{
		// Keyboard:
		// [MOD+..+]<KEYNAME> (eg. CTRL+SHIFT+K)
		// 
		// Mouse:
		// MOUSE <N>
		// MWHEELUP
		// MWHEELDOWN
		//
		// Gamepad Buttons:
		// GAMEPAD <N>
		//
		// Gamepad Axis:
		// GAMEPAD AXIS <N>
		return istream;
	}
}