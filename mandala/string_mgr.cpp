//std
#include <mutex>
#include <sstream>
#include <codecvt>

//mandala
#include "string_mgr.hpp"
#include "resource_mgr.hpp"
#include "string_archive.hpp"

namespace mandala
{
	string_mgr_t strings;

    void string_mgr_t::mount(const std::string& file)
    {
        const auto strings_lock = std::unique_lock<std::mutex>(mutex);
        const auto resources_lock = std::unique_lock<std::recursive_mutex>(resources.mutex);

        auto stream = resources.extract(hash_t(file));
        const auto archive = string_archive_t(*stream);

        streams.push_back(stream);

        for (auto& archive_language_string : archive.language_strings)
        {
			const auto& language = archive_language_string.first;
			
			auto language_strings_itr = language_strings.emplace_hint(language_strings.begin(), language, string_map_type());

			auto& strings = language_strings_itr->second;

			for (auto& archive_string : archive_language_string.second)
			{
				auto strings_itr = strings.emplace_hint(strings.begin(), archive_string.hash, string_t());
				auto& string = strings_itr->second;

				string.hash = std::move(archive_string.hash);
				string.stream_index = streams.size() - 1;
				string.offset = archive_string.offset;
			}
        }
    }

    void string_mgr_t::purge()
    {
        const auto strings_lock = std::unique_lock<std::mutex>(mutex);

        streams.clear();
		language_strings.clear();
    }

    string_mgr_t::string_type string_mgr_t::get(const hash_t& hash)
    {
        const auto strings_lock = std::unique_lock<std::mutex>(mutex);
        const auto resources_lock = std::unique_lock<std::recursive_mutex>(resources.mutex);

		auto language_strings_itr = language_strings.find(language);

		if (language_strings_itr == language_strings.end())
		{
			std::ostringstream oss;
			oss << "no strings mounted for language \"" << language << "\"";

			throw std::out_of_range(oss.str());
		}

		auto& strings = language_strings_itr->second;

        const auto strings_itr = strings.find(hash);

        if (strings_itr == strings.end())
        {
			std::ostringstream oss;
			oss << "no string \"" << hash << "\" for language \"" << language << "\"";

			throw std::out_of_range(oss.str());
        }

        const auto& string = strings_itr->second;
        const auto& stream = streams[string.stream_index];

        stream->seekg(string.offset, std::ios::beg);

		std::string buffer;
		std::getline(*stream, buffer, '\0');

        static std::wstring_convert<std::codecvt_utf8<wchar_t>> wstring_convert;

        return wstring_convert.from_bytes(buffer.data());
    }
}
