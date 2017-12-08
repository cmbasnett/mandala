#pragma once

//std
#include <map>
#include <vector>
#include <string>
#include <mutex>

//naga
#include "types.hpp"

//boost
#include <boost\shared_ptr.hpp>

namespace naga
{
    struct StringManager
    {
        typedef std::wstring StringType;
        typedef std::string LanguageType;

        struct String
        {
            u32 stream_index = 0;
            std::string key;
            u32 offset = 0;
        };

		typedef std::map<const std::string, String> StringsType;
        
		void mount(const std::string& file_name);
        void purge();
        size_t count() const;

        //TODO: template this function to allow returning of string or wstring
		StringType get(const std::string& key);

		LanguageType language; //TODO: language should be determined elsewhere (needs to be shared for other asset swapping)

    private:
        std::recursive_mutex mutex;
        std::vector<boost::shared_ptr<std::istream>> streams;
		std::map<LanguageType, StringsType> language_strings;
    };

    extern StringManager strings;
}