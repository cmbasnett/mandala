#pragma once

//mandala
#include "hash.h"
#include "resource.h"

//std
#include <map>
#include <fstream>

namespace mandala
{
	struct string_table_t : resource_t
	{
		string_table_t(std::istream& istream);

		const std::wstring& get(const hash_t& key);

	private:
		string_table_t(const string_table_t&) = delete;
		string_table_t& operator=(const string_table_t&) = delete;

		std::map<const hash_t, const std::wstring> strings;
	};
};
