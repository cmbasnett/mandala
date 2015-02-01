#pragma once

//std
#include <map>
#include <vector>
#include <memory>
#include <string>
#include <mutex>

//mandala
#include "types.hpp"
#include "hash.hpp"

namespace mandala
{
    struct string_mgr_t
    {
        typedef std::wstring string_type;
        typedef std::string language_type;

        struct string_t
        {
            uint32_t stream_index = 0;
            hash_t hash;
            uint32_t offset = 0;
		};

		typedef std::map<const hash_t, string_t> strings_type;

        std::string language = "en";

        void mount(const std::string& file);
        void purge();
        string_type get(const hash_t& hash);

    private:
        std::recursive_mutex mutex;
        std::vector<std::shared_ptr<std::istream>> streams;
		std::map<language_type, strings_type> language_strings;
    };

	extern string_mgr_t strings;
}
