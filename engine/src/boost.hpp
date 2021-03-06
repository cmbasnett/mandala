//std
#include <string>

//boost
#include <boost\optional\optional.hpp>

//naga
#include "hash.hpp"

//glm
#include <glm\glm.hpp>

namespace naga
{
    template<typename T>
    struct hash_translator
    {
        typedef std::string internal_type;
        typedef naga::details::Hash<T> external_type;

        boost::optional<external_type> get_value(const internal_type& string)
        {
            if (string.empty())
            {
                return boost::optional<external_type>(external_type());
            }

            return boost::optional<external_type>(external_type(string));
        }
    };

    template<typename T>
    struct vec2_translator
    {
        typedef std::string internal_type;
        typedef glm::tvec2<T> external_type;

        boost::optional<external_type> get_value(const internal_type& string)
        {
            if (string.empty())
            {
                return boost::optional<external_type>(boost::none);
            }

            external_type vec2;

            std::stringstream ss;
            ss.write(string);
            ss.seekg(0, std::ios_base::beg);
            ss >> vec2.x >> vec2.y;

            return vec2;
        }
    };
}

namespace boost
{
    namespace property_tree
    {
        template<typename Ch, typename Traits, typename Alloc, typename T>
        struct translator_between<std::basic_string<Ch, Traits, Alloc>, naga::details::Hash<T>>
        {
            typedef naga::hash_translator<T> type;
        };
    };
}