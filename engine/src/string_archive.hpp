#pragma once

//std
#include <istream>
#include <vector>
#include <map>

// naga
#include "types.hpp"

namespace naga
{
    struct StringArchive
    {
        struct String
        {
            std::string key;
            u32 offset;
        };

		StringArchive(std::istream& istream);

		std::map<std::string, std::vector<String>> language_strings;
    };
}
