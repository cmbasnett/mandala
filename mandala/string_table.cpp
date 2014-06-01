//mandala
#include "string_table.hpp"

namespace mandala
{
	string_table_t::string_table_t(std::istream& istream)
	{
	}

	const std::wstring& string_table_t::get(const hash_t& key)
	{
		auto strings_itr = strings.find(key);

		if (strings_itr == strings.end())
		{
			throw std::exception();
		}

		return strings_itr->second;
	}
};
