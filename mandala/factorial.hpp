#pragma once

//std
#include <type_traits>

namespace mandala
{
	template<typename T, T N, typename Enable = void>
	struct factorial;

	template<typename T, T N>
	struct factorial<T, N, typename std::enable_if<std::is_integral<T>::value && (N > 0), T>::type>
	{
		static const T value = N * factorial<T, N - 1>::value;
	};

	template<typename T>
	struct factorial<T, T(1), typename std::enable_if<std::is_integral<T>::value, T>::type>
	{
		static const T value = 1;
	};

	template<typename T>
	struct factorial<T, T(1), typename std::enable_if<std::is_enum<T> && std::is_integral<T>::value, T>::type>
	{
		static const T value = 1;
	};
}
