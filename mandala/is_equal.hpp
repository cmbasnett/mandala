#pragma once

#include <type_traits>

namespace mandala
{
	template<typename T, T N, T M, typename Enable = void>
	struct is_equal;

	template<typename T, T N, T M>
	struct is_equal<T, N, M, typename std::enable_if<std::is_integral<T>::value>::type>
	{
		static const bool value = (N == M);
	};
};
