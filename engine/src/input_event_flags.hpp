// naga
#include "types.hpp"

// stl
#include <iostream>

namespace naga
{
	typedef u8 InputEventFlagsType;

	enum : InputEventFlagsType
	{
		// TODO: this got messed up, they map directly to GLFW flags
		INPUT_EVENT_FLAG_SHIFT  = (1 << 0),
		INPUT_EVENT_FLAG_CTRL	= (1 << 1),
		INPUT_EVENT_FLAG_ALT	= (1 << 2),
		INPUT_EVENT_FLAG_SUPER	= (1 << 3)
	};

	//struct InputEventFlags
	//{
	//	InputEventFlagsType value;

	//	inline InputEventFlagsType operator()() { return value; }

	//	friend inline std::ostream& operator<<(std::ostream& ostream, InputEventFlags& flags)
	//	{
	//		return ostream;
	//	}

	//	friend inline std::istream& operator>>(std::istream& istream, InputEventFlags& flags)
	//	{
	//		return istream;
	//	}
	//};
}
