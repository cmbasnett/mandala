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
		// TODO: move this elsewhere
	}

	std::ostream& operator<<(std::ostream& ostream, const InputEvent& input_event)
	{
		ostream << input_event.type;
		return ostream;
	}
}
