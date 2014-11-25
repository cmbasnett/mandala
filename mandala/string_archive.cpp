//mandala
#include "string_archive.hpp"
#include "exceptions.hpp"

#define MAGIC_LENGTH    (4)

namespace mandala
{
    string_archive_t::string_archive_t(std::istream& istream)
    {
        //magic
        char magic[MAGIC_LENGTH + 1] = { '\0' };
        
        istream.read(magic, MAGIC_LENGTH);

		if (strcmp(magic, "LANG") != 0)
		{
			throw std::exception();
		}

        //version
		uint32_t expected_version = 1;
        uint32_t version = 0;
        istream.read(reinterpret_cast<char*>(&version), sizeof(version));

		if (version != expected_version)
        {
			throw std::exception();
        }

		//language count
		uint32_t language_count = 0;
		istream.read(reinterpret_cast<char*>(&language_count), sizeof(language_count));

		//key count
		uint32_t key_count = 0;
		istream.read(reinterpret_cast<char*>(&key_count), sizeof(key_count));

		std::vector<std::string> languages;

		for (uint32_t i = 0; i < language_count; ++i)
		{
			std::string language;
			std::getline(istream, language, '\0');

			language_strings.emplace(language, std::vector<string_t>(key_count));

			languages.emplace_back(std::move(language));
		}

        //strings
		uint32_t strings_length = 0;
		istream.read(reinterpret_cast<char*>(&strings_length), sizeof(strings_length));
		istream.seekg(strings_length, std::ios_base::cur);

		for (uint32_t i = 0; i < key_count; ++i)
        {
            std::string key;
            
			std::getline(istream, key, '\0');

			for (size_t j = 0; j < languages.size(); ++j)
			{
				const auto& language = languages[j];
				auto& strings = language_strings[language];
				auto& string = strings[i];
				
				string.hash = hash_t(key);
				istream.read(reinterpret_cast<char*>(&string.offset), sizeof(string.offset));
			}
        }
    }
}
