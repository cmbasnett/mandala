//mandala
#include "string_archive.hpp"
#include "exceptions.hpp"
#include "io.hpp"

#define LANG_MAGIC_LENGTH    (4)
#define LANG_MAGIC           (std::array<char, LANG_MAGIC_LENGTH> { 'L', 'A', 'N', 'G' })

namespace mandala
{
    string_archive_t::string_archive_t(std::istream& istream)
    {
        //magic
        std::array<char, LANG_MAGIC_LENGTH> magic;
        
        read(istream, magic);

		if (magic != LANG_MAGIC)
		{
			throw std::exception();
		}

        //version
		static const uint32_t EXPECTED_VERSION = 1;
        uint32_t version = 0;

        read(istream, version);

		if (version != EXPECTED_VERSION)
        {
			throw std::exception();
        }

		//language count
		uint32_t language_count = 0;
        read(istream, language_count);

		//key count
		uint32_t key_count = 0;
        read(istream, key_count);

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
        read(istream, strings_length);
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
                read(istream, string.offset);
			}
        }
    }
}
