#pragma once

//mandala
#include "types.hpp"

namespace mandala
{
	namespace fnv
	{
		namespace detail
		{
            template<typename T>
            struct fnv1a_t;

            template<>
            struct fnv1a_t<uint32_t>
            {
                static const uint32_t offset_basis = 2166136261UL;
                static const uint32_t prime = 16777619UL;
            };

            template<>
            struct fnv1a_t<uint64_t>
            {
                static const uint64_t offset_basis = 14695981039346656037ULL;
                static const uint64_t prime = 1099511628211ULL;
            };
		}

		template<typename T>
		T fnv1a(void* ptr, size_t length)
		{
			T hash = detail::fnv1a_t<T>::offset_basis;
			uint8_t* current = static_cast<uint8_t*>(ptr);
			uint8_t* end = current + length;

			while(current < end)
			{
				hash ^= *(current++);
                hash *= detail::fnv1a_t<T>::prime;
			}

			return hash;
		}
	}
}
