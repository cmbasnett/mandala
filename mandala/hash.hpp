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
		template<typename Value>
		struct hash_t
		{
            typedef std::string string_type;
            typedef Value value_type;
            typedef hash_t<value_type> type;

            hash_t() :
                value_(0)
            {
            }

            explicit hash_t(const string_type& string_) :
#if defined(DEBUG)
                string_(string_),
#endif
                value_(fnv::fnv1a<value_type>(reinterpret_cast<string_type::value_type*>(const_cast<string_type::value_type*>(string_.c_str())), string_.length()))
			{
			}

			hash_t(type&& copy) :
#if defined(DEBUG)
				string_(std::move(copy.string_)),
#endif
                value_(copy.value_)
			{
			}

#if defined(DEBUG)
            const string_type& string() const
            {
                return string_;
            }
#endif

            const value_type& value() const
            {
                return value_;
            }

		private:
#if defined(DEBUG)
            string_type string_;
#endif
            value_type value_ = value_type(0);

		public:
			hash_t& operator=(const hash_t& rhs)
			{
#if defined(DEBUG)
                string_ = rhs.string_;
#endif
                value_ = rhs.value_;

				return *this;
			}

			type& operator=(string_type&& string__)
			{
#if defined(DEBUG)
                string_ = string__;
#endif
                value_(fnv::fnv1a<value_type>(reinterpret_cast<char*>(const_cast<char*>(string.c_str())), string.length()))

				return *this;
			}

			inline type& operator=(type&& copy)
			{
#if defined(DEBUG)
				string_ = std::move(copy.string_);
#endif
				value_ = copy.value_;

				return *this;
			}

			inline bool operator==(const type& rhs) const { return value_ == rhs.value_; }
			inline bool operator!=(const type& rhs) const { return value_ != rhs.value_; }
			inline bool operator>(const type& rhs) const { return value_ > rhs.value_; }
			inline bool operator<(const type& rhs) const { return value_ < rhs.value_; }
			inline bool operator>=(const type& rhs) const { return value_ >= rhs.value_; }
			inline bool operator<=(const type& rhs) const { return value_ <= rhs.value_; }
		};
	}

	typedef details::hash_t<uint32_t> hash32_t;
	typedef details::hash_t<uint64_t> hash64_t;
	typedef hash32_t hash_t;
}

template<typename T>
std::ostream& operator<<(std::ostream& ostream, const mandala::details::hash_t<T>& hash)
{
    return ostream << hash.value();
}
