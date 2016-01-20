#pragma once

//std
#include <istream>
#include <vector>
#include <map>

//naga
#include "hash.hpp"

namespace naga
{
    struct string_archive
    {
        struct string
        {
            hash hash;
            u32 offset;
        };

        string_archive(std::istream& istream);

        std::map<std::string, std::vector<string>> language_strings;
    };
}
