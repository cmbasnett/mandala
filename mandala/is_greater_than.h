#pragma once

#include <type_traits>

namespace mandala
{
	template<typename T, T N, T M, typename Enable = void>
	struct is_greater_than;

	template<typename T, T N, T M>
	struct is_greater_than<T, N, M, typename std::enable_if<std::is_integral<T>::value>::type>
	{
		static const bool value = (N > M);
	};
};