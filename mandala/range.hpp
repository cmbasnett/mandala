#pragma once

//std
#include <type_traits>

namespace mandala
{
    template<typename T, typename Enable = void>
	struct range_t;

	template<typename T>
	struct range_t<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
	{
		T min;
		T max;
	};
}
