#pragma once

#include <string>
#include <map>
#include <mutex>

#include <boost\shared_ptr.hpp>

namespace mandala
{
    struct cache_mgr_t
    {
        cache_mgr_t();
        ~cache_mgr_t();

        std::unique_ptr<std::ifstream> get(const std::string& file_name) const;
        int32_t put(const std::string& file_name, const void* buffer, size_t count);
        void erase(const std::string& file_name);
        int32_t checksum(const std::string& file_name) const;
        void purge();
        void write();
        void read();

    private:
        std::map<std::string, int32_t> file_checksums;
        std::mutex mutex;
    };

    extern cache_mgr_t cache;
}