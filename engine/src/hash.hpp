#pragma once

//naga
#include "types.hpp"
#include "fnv.hpp"

//std
#include <string>

namespace naga
{
    namespace details
    {
        template<typename ValueType>
        struct Hash
        {
            typedef std::string StringType;
			typedef Hash<ValueType> Type;

			Hash() :
                value(0)
            {
            }

			explicit Hash(const StringType& string) :
#if defined(DEBUG)
                string(string),
#endif
				value(fnv1a<ValueType>(reinterpret_cast<StringType::value_type*>(const_cast<StringType::value_type*>(string.c_str())), string.length()))
            {
            }

			Hash(Type&& copy) :
#if defined(DEBUG)
                string(std::move(copy.string)),
#endif
                value(copy.value)
            {
            }

#if defined(DEBUG)
            const StringType& get_string() const
            {
                return string;
            }
#endif

            const ValueType& get_value() const
            {
                return value;
            }

        private:
#if defined(DEBUG)
            StringType string;
#endif
			ValueType value = ValueType(0);

        public:
			Hash& operator=(const Hash& rhs)
            {
#if defined(DEBUG)
                string = rhs.string;
#endif
                value = rhs.value;

                return *this;
            }

			Type& operator=(StringType&& string)
            {
#if defined(DEBUG)
                this->string = string;
#endif
				value(fnv1a<ValueType>(reinterpret_cast<char*>(const_cast<char*>(string.c_str())), string.length()))

                return *this;
            }

			inline Type& operator=(Type&& copy)
            {
#if defined(DEBUG)
                string = std::move(copy.string);
#endif
                value = copy.value;

                return *this;
            }

			inline bool operator==(const Type& rhs) const { return value == rhs.value; }
			inline bool operator!=(const Type& rhs) const { return value != rhs.value; }
			inline bool operator>(const Type& rhs) const { return value > rhs.value; }
			inline bool operator<(const Type& rhs) const { return value < rhs.value; }
			inline bool operator>=(const Type& rhs) const { return value >= rhs.value; }
			inline bool operator<=(const Type& rhs) const { return value <= rhs.value; }
        };
    }

    typedef details::Hash<u32> hash_u32;
	typedef details::Hash<u64> hash_u64;
    typedef hash_u32 hash;
}

template<typename T>
std::ostream& operator<<(std::ostream& ostream, const naga::details::Hash<T>& hash)
{
    return ostream << hash.get_value();
}