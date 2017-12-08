#pragma once

//naga
#include "types.hpp"

namespace naga
{
	namespace detail
	{
		template<typename Value>
		struct FNV1A;

		template<>
		struct FNV1A<u32>
		{
			typedef u32 ValueType;

			static const ValueType OFFSET_BASIS = 2166136261UL;
			static const ValueType PRIME = 16777619UL;
		};

		template<>
		struct FNV1A<u64>
		{
			typedef u64 ValueType;

			static const ValueType OFFSET_BASIS = 14695981039346656037ULL;
			static const ValueType PRIME = 1099511628211ULL;
		};
	}

	template<typename ValueType>
	ValueType fnv1a(void* ptr, size_t length)
	{
		ValueType hash = detail::FNV1A<ValueType>::OFFSET_BASIS;
		auto current = static_cast<u8*>(ptr);
		auto end = current + length;

		while (current < end)
		{
			hash ^= *(current++);
			hash *= detail::FNV1A<ValueType>::PRIME;
		}

		return hash;
	}
}
