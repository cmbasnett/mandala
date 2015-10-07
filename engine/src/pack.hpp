#pragma once

//std
#include <map>

//mandala
#include "hash.hpp"

//boost
#include <boost\shared_ptr.hpp>
#include <boost\iostreams\device\mapped_file.hpp>

namespace mandala
{
    struct pack
    {
        struct file
        {
            hash pack_hash;
            std::string name;
            u32 offset = 0;
            u32 length = 0;
            u32 crc32 = 0;
        };

        typedef std::map<const hash, file> files_type;

        pack(const std::string& path);
        pack(pack&& rhs);

        std::string path;
        files_type files;
        boost::iostreams::mapped_file_source mapped_file_source;

        pack& operator=(pack&& rhs);

    private:

        pack(const pack&) = delete;
        pack& operator=(const pack&) = delete;
    };
}
