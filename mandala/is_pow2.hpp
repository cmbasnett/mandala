#pragma once

//std
#include <type_traits>

namespace mandala
{
	template<typename T, T N, typename Enable = void>
	struct is_pow2;

	template<typename T, T N>
	struct is_pow2<T, N, typename std::enable_if<std::is_unsigned<T>::value>::type>
	{
		static const bool value = (N != 0 && (N & (~N + 1)) == N);
	};

	template<typename T>
	inline bool is_pow2(T N)
	{
		return (N != 0 && (N & (~N + 1)) == N);
	}
}
