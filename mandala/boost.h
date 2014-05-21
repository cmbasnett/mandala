//std
#include <string>

//boost
#include <boost\optional\optional.hpp>

//mandala
#include "hash.h"

namespace mandala
{
	template<typename T>
	struct hash_translator
	{
		typedef std::string internal_type;
		typedef mandala::details::hash_t<T> external_type;

		boost::optional<external_type> get_value(const internal_type& string)
		{
			if (!string.empty())
			{
				return boost::optional<external_type>(external_type(string));
			}
			else
			{
				return boost::optional<external_type>(boost::none);
			}
		}
	};
};

namespace boost
{
	namespace property_tree
	{
		template<typename Ch, typename Traits, typename Alloc, typename T>
		struct translator_between<std::basic_string<Ch, Traits, Alloc>, mandala::details::hash_t<T>>
		{
			typedef mandala::hash_translator<T> type;
		};
	};
};
