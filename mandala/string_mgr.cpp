//std
#include <mutex>
#include <sstream>
#include <codecvt>

//mandala
#include "string_mgr.hpp"
#include "app.hpp"
#include "string_archive.hpp"

namespace mandala
{
    void string_mgr_t::mount(const std::string& file)
    {
        const auto strings_lock = std::unique_lock<std::mutex>(mutex);
        const auto resources_lock = std::unique_lock<std::recursive_mutex>(app.resources.mutex);

        auto stream = app.resources.extract(hash_t(file));
        const auto archive = string_archive_t(*stream);

        streams.push_back(stream);

        for (auto& archive_string : archive.strings)
        {
            string_t string;
            string.hash = std::move(archive_string.hash);
            string.stream_index = streams.size() - 1;
            string.offset = archive_string.offset;
            string.length = archive_string.length;

            strings.insert(std::make_pair(string.hash, string));
        }
    }

    void string_mgr_t::purge()
    {
        const auto strings_lock = std::unique_lock<std::mutex>(mutex);

        streams.clear();
        strings.clear();
    }

    string_mgr_t::string_type string_mgr_t::get(const hash_t& hash)
    {
        const auto strings_lock = std::unique_lock<std::mutex>(mutex);
        const auto resources_lock = std::unique_lock<std::recursive_mutex>(app.resources.mutex);

        const auto strings_itr = strings.find(hash);

        if (strings_itr == strings.end())
        {
            throw std::out_of_range("");
        }

        const auto& string = strings_itr->second;
        const auto& stream = streams[string.stream_index];

        stream->seekg(string.offset, std::ios::beg);

        std::vector<char> buffer;
        buffer.resize(string.length);
        stream->read(buffer.data(), string.length);

        static std::wstring_convert<std::codecvt_utf16<wchar_t>> wstring_convert;

        return wstring_convert.from_bytes(buffer.data());
    }
}
