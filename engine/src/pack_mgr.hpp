#pragma once

//std
#include <mutex>

//mandala
#include "hash.hpp"
#include "pack.hpp"

namespace mandala
{
    struct pack_mgr
    {
        void mount(const std::string& path);
        void unmount_all();

        boost::shared_ptr<std::istream> extract(const hash& hash);

    private:
        std::recursive_mutex mutex;
        std::map<const hash, pack::file> files;
        std::map<const hash, pack> packs;
    };
}
