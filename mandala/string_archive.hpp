#pragma once

//std
#include <istream>
#include <vector>

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
			uint32_t length;
		};

        string_archive_t(std::istream& istream);

        std::vector<string_t> strings;
    };
}
