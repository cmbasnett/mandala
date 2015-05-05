#pragma once

//mandala
#include "types.hpp"

namespace mandala
{
	namespace fnv
	{
		namespace detail
		{
            template<typename Value>
            struct fnv1a_t;

            template<>
            struct fnv1a_t<uint32_t>
            {
                typedef uint32_t value_type;

                static const value_type OFFSET_BASIS = 2166136261UL;
                static const value_type PRIME = 16777619UL;
            };

            template<>
            struct fnv1a_t<uint64_t>
            {
                typedef uint64_t value_type;

                static const value_type OFFSET_BASIS = 14695981039346656037ULL;
                static const value_type PRIME = 1099511628211ULL;
            };
		}

		template<typename Value>
        Value fnv1a(void* ptr, size_t length)
		{
			Value hash = detail::fnv1a_t<Value>::OFFSET_BASIS;
			auto current = static_cast<uint8_t*>(ptr);
			auto end = current + length;

			while(current < end)
			{
				hash ^= *(current++);
                hash *= detail::fnv1a_t<Value>::PRIME;
			}

			return hash;
		}
	}
}
