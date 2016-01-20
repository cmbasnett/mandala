//naga
#include "string_archive.hpp"
#include "exceptions.hpp"
#include "io.hpp"

#define LANG_MAGIC_LENGTH    (4)
#define LANG_MAGIC           (std::array<char, LANG_MAGIC_LENGTH> { { 'L', 'A', 'N', 'G' } })

namespace naga
{
    string_archive::string_archive(std::istream& istream)
    {
        //magic
        std::array<char, LANG_MAGIC_LENGTH> magic;
        
        read(istream, magic);

        if (magic != LANG_MAGIC)
        {
            throw std::exception();
        }

        //version
        static const u32 EXPECTED_VERSION = 1;
        u32 version = 0;

        read(istream, version);

        if (version != EXPECTED_VERSION)
        {
            throw std::exception();
        }

        //language count
        u32 language_count = 0;
        read(istream, language_count);

        //key count
        u32 key_count = 0;
        read(istream, key_count);

        std::vector<std::string> languages;

        for (u32 i = 0; i < language_count; ++i)
        {
            std::string language;
            std::getline(istream, language, '\0');

            language_strings.emplace(language, std::vector<string>(key_count));

            languages.emplace_back(std::move(language));
        }

        //strings
        u32 strings_length = 0;
        read(istream, strings_length);
        istream.seekg(strings_length, std::ios_base::cur);

        for (u32 i = 0; i < key_count; ++i)
        {
            std::string key;
            
            std::getline(istream, key, '\0');

            for (size_t j = 0; j < languages.size(); ++j)
            {
                const auto& language = languages[j];
                auto& strings = language_strings[language];
                auto& string = strings[i];
                
                string.hash = hash(key);
                read(istream, string.offset);
            }
        }
    }
}
