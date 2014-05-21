#pragma once

//mandala
#include "types.h"

namespace mandala
{
	namespace fnv
	{
		namespace detail
		{
			template<typename T>
			struct offset_basis_t;

			template<>
			struct offset_basis_t<uint32_t>
			{
				static const uint32_t value = 2166136261UL;
			};
			
			template<>
			struct offset_basis_t<uint64_t>
			{
				static const uint64_t value = 14695981039346656037ULL;
			};

			template<typename T>
			struct prime_t;
			
			template<>
			struct prime_t<uint32_t>
			{
				static const uint32_t value = 16777619UL;
			};
			
			template<>
			struct prime_t<uint64_t>
			{
				static const uint64_t value = 1099511628211ULL;
			};

			template<typename T, int L>
			struct fnv1a_t
			{
				static const T value = L + fnv1a_t<T, L - 1>::value;
			};

			template<typename T>
			struct fnv1a_t<T, 0>
			{
				static const T value = 0;
			};

		}

		template<typename T>
		T fnv1a(void* ptr, size_t length)
		{
			T hash = detail::offset_basis_t<T>::value;
			uint8_t* current = static_cast<uint8_t*>(ptr);
			uint8_t* end = current + length;

			while(current < end)
			{
				hash ^= *(current++);
				hash *= detail::prime_t<T>::value;
			}

			return hash;
		}
	};
};