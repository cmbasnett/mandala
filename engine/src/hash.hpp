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
                value(0)
            {
            }

            explicit hash_t(const string_type& string) :
#if defined(DEBUG)
                string(string),
#endif
                value(fnv::fnv1a<value_type>(reinterpret_cast<string_type::value_type*>(const_cast<string_type::value_type*>(string.c_str())), string.length()))
            {
            }

            hash_t(type&& copy) :
#if defined(DEBUG)
                string(std::move(copy.string)),
#endif
                value(copy.value)
            {
            }

#if defined(DEBUG)
            const string_type& get_string() const
            {
                return string;
            }
#endif

            const value_type& get_value() const
            {
                return value;
            }

        private:
#if defined(DEBUG)
            string_type string;
#endif
            value_type value = value_type(0);

        public:
            hash_t& operator=(const hash_t& rhs)
            {
#if defined(DEBUG)
                string = rhs.string;
#endif
                value = rhs.value;

                return *this;
            }

            type& operator=(string_type&& string)
            {
#if defined(DEBUG)
                this->string = string;
#endif
                value(fnv::fnv1a<value_type>(reinterpret_cast<char*>(const_cast<char*>(string.c_str())), string.length()))

                return *this;
            }

            inline type& operator=(type&& copy)
            {
#if defined(DEBUG)
                string = std::move(copy.string);
#endif
                value = copy.value;

                return *this;
            }

            inline bool operator==(const type& rhs) const { return value == rhs.value; }
            inline bool operator!=(const type& rhs) const { return value != rhs.value; }
            inline bool operator>(const type& rhs) const { return value > rhs.value; }
            inline bool operator<(const type& rhs) const { return value < rhs.value; }
            inline bool operator>=(const type& rhs) const { return value >= rhs.value; }
            inline bool operator<=(const type& rhs) const { return value <= rhs.value; }
        };
    }

    typedef details::hash_t<u32> hash32_t;
    typedef details::hash_t<u64> hash64_t;
    typedef hash32_t hash_t;
}

template<typename T>
std::ostream& operator<<(std::ostream& ostream, const mandala::details::hash_t<T>& hash)
{
    return ostream << hash.get_value();
}