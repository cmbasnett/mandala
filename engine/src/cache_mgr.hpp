#pragma once

//std
#include <string>
#include <map>
#include <mutex>

//boost
#include <boost\shared_ptr.hpp>

//naga
#include "types.hpp"

namespace naga
{
    struct cache_mgr
    {
        cache_mgr();
        ~cache_mgr();

        std::unique_ptr<std::ifstream> get(const std::string& file_name) const;
        i32 put_buffer(const std::string& file_name, const void* buffer, size_t count);
        i32 put(const std::string& file_name, const std::string& contents);
        void erase(const std::string& file_name);
        i32 checksum(const std::string& file_name) const;
        void purge();
        void write();
        void read();

    private:
        std::map<std::string, i32> file_checksums;
        std::mutex mutex;
    };

    extern cache_mgr cache;
}