#pragma once

//std
#include <map>
#include <vector>
#include <string>
#include <mutex>

//mandala
#include "types.hpp"
#include "hash.hpp"

//boost
#include <boost\shared_ptr.hpp>

namespace mandala
{
    struct string_mgr_t
    {
        typedef std::wstring string_type;
        typedef std::string language_type;

        struct string_t
        {
            u32 stream_index = 0;
            hash_t hash;
            u32 offset = 0;
        };

        typedef std::map<const hash_t, string_t> strings_type;
        
        void mount(const std::string& file);
        void purge();
        size_t count() const;

        //TODO: template this function to allow returning of string or wstring
        string_type get(const hash_t& hash);

        language_type language; //TODO: language should be determined elsewhere (needs to be shared for other asset swapping)

    private:
        std::recursive_mutex mutex;
        std::vector<boost::shared_ptr<std::istream>> streams;
        std::map<language_type, strings_type> language_strings;
    };

    extern string_mgr_t strings;
}