#pragma once

//naga
#include "types.hpp"

namespace naga
{
    namespace fnv
    {
        namespace detail
        {
            template<typename Value>
            struct fnv1a;

            template<>
            struct fnv1a<u32>
            {
                typedef u32 value_type;

                static const value_type OFFSET_BASIS = 2166136261UL;
                static const value_type PRIME = 16777619UL;
            };

            template<>
            struct fnv1a<u64>
            {
                typedef u64 value_type;

                static const value_type OFFSET_BASIS = 14695981039346656037ULL;
                static const value_type PRIME = 1099511628211ULL;
            };
        }

        template<typename Value>
        Value fnv1a(void* ptr, size_t length)
        {
            Value hash = detail::fnv1a<Value>::OFFSET_BASIS;
            auto current = static_cast<u8*>(ptr);
            auto end = current + length;

            while(current < end)
            {
                hash^= *(current++);
                hash*= detail::fnv1a<Value>::PRIME;
            }

            return hash;
        }
    }
}
