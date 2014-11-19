//mandala
#include "string_archive.hpp"

#define MAGIC_LENGTH    (4)
#define VERSION			(1)

namespace mandala
{
    string_archive_t::string_archive_t(std::istream& istream)
    {
        //magic
        char magic[MAGIC_LENGTH] = { '\0' };
        
        istream.read(magic, MAGIC_LENGTH);

        if (strcmp(magic, "TEXT") != 0)
        {
            throw std::exception();
        }

        //version
        uint32_t version = 0;

		istream.read(reinterpret_cast<char*>(&version), sizeof(version));

		if (version != VERSION)
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
            
            std::getline(istream, string_name, '\0');

            string.hash = hash_t(string_name);
            
            istream.read(reinterpret_cast<char*>(&string.offset), sizeof(string.offset));
            istream.read(reinterpret_cast<char*>(&string.length), sizeof(string.length));
        }
    }
}
