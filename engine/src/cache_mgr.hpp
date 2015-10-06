#pragma once

//std
#include <string>
#include <map>
#include <mutex>

//boost
#include <boost\shared_ptr.hpp>

//mandala
#include "types.hpp"

namespace mandala
{
    struct cache_mgr_t
    {
        cache_mgr_t();
        ~cache_mgr_t();

        std::unique_ptr<std::ifstream> get(const std::string& file_name) const;
        i32 put(const std::string& file_name, const void* buffer, size_t count);
        void erase(const std::string& file_name);
        i32 checksum(const std::string& file_name) const;
        void purge();
        void write();
        void read();

    private:
        std::map<std::string, i32> file_checksums;
        std::mutex mutex;
    };

    extern cache_mgr_t cache;
}