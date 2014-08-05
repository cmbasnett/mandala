//mandala
#include "string_archive.hpp"

namespace mandala
{
    string_archive_t::string_archive_t(std::istream& istream)
    {
        //magic
        char magic[4] = { '\0' };
        
        istream.read(magic, sizeof(magic));

        if (strcmp(magic, "LOCA") != 0)
        {
            throw std::exception();
        }

        //version
        uint32_t version = 0;

        istream.read(reinterpret_cast<char*>(&version), sizeof(version));

        if (version != 1)
        {
            throw std::exception();
        }

        //string count
        uint32_t string_count = 0;

        istream.read(reinterpret_cast<char*>(&string_count), sizeof(string_count));

        strings.resize(string_count);

        for (auto& string : strings)
        {
            std::string string_name;
            
            std::getline(istream, string_name, '\n');

            string.hash = hash_t(string_name);
            
            istream.read(reinterpret_cast<char*>(&string.offset), sizeof(string.offset));
            istream.read(reinterpret_cast<char*>(&string.length), sizeof(string.length));
        }
    }
}
