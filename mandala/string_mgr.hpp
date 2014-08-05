#pragma once

//std
#include <map>
#include <vector>
#include <memory>
#include <string>

//mandala
#include "types.hpp"
#include "hash.hpp"

namespace mandala
{
    struct string_mgr_t
    {
		static std::string default_language;

        typedef std::wstring string_type;

        struct string_t
        {
            uint32_t stream_index = 0;
            hash_t hash;
            uint32_t offset = 0;
            uint32_t length = 0;
        };

		string_mgr_t(const std::string& language = default_language);

        std::string language;
        std::vector<std::shared_ptr<std::istream>> streams;
        std::map<hash_t, string_t> strings;

        void mount(const std::string& file);
        void purge();
        string_type get(const hash_t& hash) const;
    };
}
