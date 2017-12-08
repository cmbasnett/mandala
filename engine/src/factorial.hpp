#pragma once

//std
#include <type_traits>

namespace naga
{
    template<typename T, T N, typename Enable = void>
    struct Factorial;

    template<typename T, T N>
	struct Factorial<T, N, typename std::enable_if<std::is_integral<T>::value && (N > 0), T>::type>
    {
        static const T VALUE = N * factorial<T, N - 1>::VALUE;
    };

    template<typename T>
	struct Factorial<T, T(1), typename std::enable_if<std::is_integral<T>::value, T>::type>
    {
        static const T VALUE = 1;
    };

    template<typename T>
	struct Factorial<T, T(1), typename std::enable_if<std::is_enum<T> && std::is_integral<T>::value, T>::type>
    {
        static const T VALUE = 1;
    };
}
