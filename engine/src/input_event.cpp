// std
#include <vector>
#include <boost\algorithm\string.hpp>

// naga
#include "input_event.hpp"

#include <boost\lexical_cast.hpp>

namespace naga
{
	InputEvent::InputEvent(const std::string& string)
	{
		std::vector<std::string> tokens;
		boost::split(tokens, string, boost::is_any_of(" "), boost::algorithm::token_compress_on);

		auto device_type = boost::lexical_cast<InputDeviceType>(tokens[0]);

		switch (device_type)
		{
		case InputDeviceType::GAMEPAD:
			if (tokens[1] == "Axis")
			{
				gamepad.type = Gamepad::Type::AXIS_MOVE;
				gamepad.axis_index = std::stoi(tokens[2]);
			}
			else
			{
				gamepad.type = Gamepad::Type::PRESS;
				gamepad.button_index = std::stoi(tokens[1]);
			}
			break;
		case InputDeviceType::KEYBOARD:

			break;
		case InputDeviceType::MOUSE:
			break;
		default:
			throw std::exception();
		}
	}

	std::ostream& operator<<(std::ostream& ostream, const InputEvent& input_event)
	{
		ostream << input_event.device_type;
		return ostream;
	}
}
