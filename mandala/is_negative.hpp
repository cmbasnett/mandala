#pragma once

//std
#include <type_traits>

namespace mandala
{
	template<typename T, T N, typename Enable = void>
	struct is_negative;
	
	template<typename T, T N>
	struct is_negative<T, N, typename std::enable_if<std::is_unsigned<T>::value>::type>
	{
		static const bool VALUE = false;
	};

	template<typename T, T N>
	struct is_negative<T, N, typename std::enable_if<std::is_integral<T>::value && !std::is_unsigned<T>::value>::type>
	{
		static const bool VALUE = N < 0;
	};

	template<typename T, T N>
	struct is_negative<T, N, typename std::enable_if<std::is_enum<T>::value && std::is_integral<std::underlying_type<T>::value>::value>::type>
	{
		static const bool VALUE = is_negative<std::underlying_type<T>::value(N)>::value;
	};
}
