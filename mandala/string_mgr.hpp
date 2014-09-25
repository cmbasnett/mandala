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

        struct string_t
        {
            uint32_t stream_index = 0;
            hash_t hash;
            uint32_t offset = 0;
            uint32_t length = 0;
        };

        std::string language = "en";

        void mount(const std::string& file);
        void purge();
        string_type get(const hash_t& hash);

    private:
        std::mutex mutex;
        std::vector<std::shared_ptr<std::istream>> streams;
        std::map<const hash_t, const string_t> strings;
    };
}
