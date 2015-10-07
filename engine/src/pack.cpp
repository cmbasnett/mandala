//mandala
#include "pack.hpp"
#include "io.hpp"

//std
#include <fstream>
#include <sstream>

//boost
#include <boost\iostreams\stream.hpp>
#include <boost\filesystem.hpp>

#define PACK_MAGIC_LENGTH   (4)
#define PACK_MAGIC          (std::array<char, PACK_MAGIC_LENGTH> { { 'P', 'A', 'C', 'K' } })
#define PACK_VERSION        (1)

namespace mandala
{
    pack::pack(const std::string& path) :
        path(path)
    {
        auto istream = std::ifstream(path, std::ios_base::binary);

        //magic
        std::array<char, PACK_MAGIC_LENGTH> magic;
        read(istream, magic);

        if (magic != PACK_MAGIC)
        {
            throw std::exception();
        }

        //version
        u32 version = 0;
        read(istream, version);

        if (version != PACK_VERSION)
        {
            throw std::exception();
        }

        //file count
        u32 file_count = 0;
        read(istream, file_count);

        for(u32 i = 0; i < file_count; ++i)
        {
            file file;

            std::getline(istream, file.name, '\0');

            read(istream, file.offset);
            read(istream, file.length);
            read(istream, file.crc32);

            auto files_itr = files.emplace(hash(file.name), std::forward<pack::file>(file));

            if (!files_itr.second)
            {
                throw std::exception("duplicate file in pack");
            }
        }

        mapped_file_source = boost::iostreams::mapped_file_source(path);
    }

    pack::pack(pack&& copy) :
        path(std::move(copy.path)),
        files(std::move(copy.files)),
        mapped_file_source(std::move(copy.mapped_file_source))
    {
    }

    pack& pack::operator=(pack&& copy)
    {
        path = std::move(copy.path);
        files = std::move(copy.files);
        mapped_file_source = std::move(copy.mapped_file_source);

        return *this;
    }
}
