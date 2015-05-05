#pragma once

//std
#include <type_traits>

namespace mandala
{
	template<typename T, T N, T M, typename Enable = void>
	struct is_less_than_or_equal;

	template<typename T, T N, T M>
	struct is_less_than_or_equal<T, N, M, typename std::enable_if<std::is_integral<T>::value>::type>
	{
		static const bool VALUE = (N <= M);
	};
}
