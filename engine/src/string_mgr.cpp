//std
#include <mutex>
#include <sstream>
#include <codecvt>

//naga
#include "string_mgr.hpp"
#include "resource_manager.hpp"
#include "string_archive.hpp"

namespace naga
{
    StringManager strings;

	void StringManager::mount(const std::string& file_name)
    {
        std::lock_guard<std::recursive_mutex> lock(mutex);

		auto stream = resources.extract(file_name);
        const auto archive = StringArchive(*stream);

        streams.push_back(stream);

        for (const auto& archive_language_string : archive.language_strings)
        {
            const auto& language = archive_language_string.first;
            
			auto language_strings_itr = language_strings.emplace_hint(language_strings.begin(), language, StringsType());

            auto& strings = language_strings_itr->second;

            for (auto& archive_string : archive_language_string.second)
            {
                auto strings_itr = strings.emplace_hint(strings.begin(), archive_string.key, String());
                auto& string = strings_itr->second;

                string.key = std::move(archive_string.key);
                string.stream_index = streams.size() - 1;
                string.offset = archive_string.offset;
            }
        }
    }

    void StringManager::purge()
    {
        std::lock_guard<std::recursive_mutex> lock(mutex);

        streams.clear();
        language_strings.clear();
    }

	size_t StringManager::count() const
    {
        auto language_strings_itr = language_strings.find(language);

        if (language_strings_itr == language_strings.end())
        {
            return 0;
        }

        return language_strings_itr->second.size();
    }

	StringManager::StringType StringManager::get(const std::string& key)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> wstring_convert;

        std::lock_guard<std::recursive_mutex> lock(mutex);

        auto language_strings_itr = language_strings.find(language);

        if (language_strings_itr == language_strings.end())
        {
            std::ostringstream oss;
            oss << "no strings mounted for language \"" << language << "\"";
            throw std::out_of_range(oss.str());
        }

        auto& strings = language_strings_itr->second;

		const auto strings_itr = strings.find(key);

        if (strings_itr == strings.end())
        {
            std::ostringstream oss;
			oss << "no string \"" << key << "\" for language \"" << language << "\"";
            throw std::out_of_range(oss.str());
        }

        const auto& string = strings_itr->second;
        const auto& stream = streams[string.stream_index];

        stream->seekg(string.offset, std::ios::beg);

        std::string buffer;
        std::getline(*stream, buffer, '\0');

        //string expansion
        auto beg = 0;
        auto end = std::string::npos;

        for (;;)
        {
            //TODO: use regular expressions
            beg = buffer.find("{$", beg);

            if (beg == std::string::npos)
            {
                break;
            }

            end = buffer.find('}', beg);

            if (end == std::string::npos)
            {
                break;
            }

            auto key = buffer.substr(beg + 2, end - beg - 2);

            try
            {
                auto string = wstring_convert.to_bytes(get(key));

                buffer.replace(beg, end - beg + 1, string);

                beg += string.length();
            }
            catch (std::out_of_range&)
            {
                //TODO: warn that string expansion failed
                beg = end;
            }
        }

        return wstring_convert.from_bytes(buffer.data());
    }
}