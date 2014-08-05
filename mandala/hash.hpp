#pragma once

//mandala
#include "types.hpp"
#include "fnv.hpp"

//std
#include <string>

namespace mandala
{
	namespace details
	{
		template<typename T>
		struct hash_t
		{
			typedef T value_type;

			hash_t() :
				value(0)
			{
			}

			explicit hash_t(const std::string& string) :
#ifdef _DEBUG
				string(string),
#endif //_DEBUG
				value(fnv::fnv1a<T>((void*)string.c_str(), string.length()))
			{
			}

			hash_t(hash_t<T>&& copy) :
#ifdef _DEBUG
				string(std::move(copy.string)),
#endif //_DEBUG
				value(copy.value)
			{
			}

		private:
#ifdef _DEBUG
			std::string string;
#endif //_DEBUG
			T value = T(0);

		public:
			hash_t& operator=(const hash_t& rhs)
			{
#ifdef _DEBUG
				string = rhs.string;
#endif //_DEBUG
				value = rhs.value;

				return *this;
			}

			hash_t& operator=(std::string&& string_)
			{
#ifdef _DEBUG
				string = string_;
#endif //_DEBUG
				value = fnv::fnv1a<T>((void*)(string_.c_str()), string_.length());

				return *this;
			}

			inline hash_t& operator=(hash_t<T>&& copy)
			{
#ifdef _DEBUG
				string = std::move(copy.string);
#endif //_DEBUG
				value = copy.value;

				return *this;
			}

			inline bool operator==(const hash_t& rhs) const { return value == rhs.value; }
			inline bool operator!=(const hash_t& rhs) const { return value != rhs.value; }
			inline bool operator>(const hash_t& rhs) const { return value > rhs.value; }
			inline bool operator<(const hash_t& rhs) const { return value < rhs.value; }
			inline bool operator>=(const hash_t& rhs) const { return value >= rhs.value; }
			inline bool operator<=(const hash_t& rhs) const { return value <= rhs.value; }
		};
	}

	typedef details::hash_t<uint32_t> hash32_t;
	typedef details::hash_t<uint64_t> hash64_t;
	typedef hash32_t hash_t;
}
