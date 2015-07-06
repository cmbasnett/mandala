#pragma once

//std
#include <istream>
#include <vector>
#include <map>

//mandala
#include "hash.hpp"

namespace mandala
{
    struct string_archive_t
	{
		struct string_t
		{
			hash_t hash;
			uint32_t offset;
		};

        string_archive_t(std::istream& istream);

		std::map<std::string, std::vector<string_t>> language_strings;
    };
}
