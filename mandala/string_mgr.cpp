//std
#include <mutex>
#include <sstream>
#include <codecvt>

//mandala
#include "string_mgr.h"
#include "app.hpp"
#include "string_archive.h"

namespace mandala
{
    string_mgr_t::string_mgr_t(const std::string& language) :
        language(language)
    {
    }

    void string_mgr_t::mount(const std::string& file)
    {
        auto lock_guard = std::unique_lock<std::recursive_mutex>(app.resources.mutex);

        auto stream_hash = hash_t(file);

        std::shared_ptr<std::istream> stream;

        stream = app.resources.extract(stream_hash);

        auto string_archive = string_archive_t(*stream);

        streams.push_back(stream);

        for (auto& _string : string_archive.strings)
        {
             string_t string;
             string.hash = std::move(_string.hash);
             string.stream_index = streams.size() - 1;
             string.offset = _string.offset;
             string.length = _string.length;

             strings.insert(std::make_pair(string.hash, string));
        }
    }

    void string_mgr_t::unmount_all()
    {
        streams.clear();
        strings.clear();
    }

    string_mgr_t::string_type string_mgr_t::get(const hash_t& hash) const
    {
        auto strings_itr = strings.find(hash);

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
};
