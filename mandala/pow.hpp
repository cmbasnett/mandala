#pragma once

#include <type_traits>

namespace mandala
{
	namespace type_traits
	{
		template<int Base, int Exponent>
		struct pow
		{
			static const int value = Base * pow<Base, Exponent - 1>::value;
		};

		template<int Base>
		struct pow<Base, 0>
		{
			static const int value = 1;
		};
	};
};
