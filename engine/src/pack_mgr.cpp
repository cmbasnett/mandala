//mandala
#include "pack_mgr.hpp"
#include "pack.hpp"

//std
#include <sstream>
#include <strstream>

//boost
#include <boost\filesystem\path.hpp>
#include <boost\make_shared.hpp>

namespace mandala
{
    void pack_mgr::mount(const std::string& path)
    {
        std::lock_guard<std::recursive_mutex> lock(mutex);

        const auto pack_string = boost::filesystem::path(path).filename().string();
        const auto pack_hash = hash(pack_string);

        packs.erase(pack_hash);

        auto packs_itr = packs.insert(std::make_pair(pack_hash, pack(path)));

        if (!packs_itr.second)
        {
            std::ostringstream ostringstream;
            ostringstream << "pack \"" << pack_string << "\" already mounted";

            throw std::exception(ostringstream.str().c_str());
        }

        auto& pack = packs_itr.first->second;

        for (auto& file : pack.files)
        {
            file.second.pack_hash = pack_hash;

            files[file.first] = file.second;
        }
    }

    void pack_mgr::unmount_all()
    {
        std::lock_guard<std::recursive_mutex> lock(mutex);

        files.clear();
        packs.clear();
    }

    boost::shared_ptr<std::istream> pack_mgr::extract(const hash& hash)
    {
        std::lock_guard<std::recursive_mutex> lock(mutex);

        auto files_itr = files.find(hash);

        if (files_itr == files.end())
        {
            std::ostringstream ostringstream;
            ostringstream << "no such file " << hash;

            throw std::out_of_range(ostringstream.str().c_str());
        }

        const auto& file = files_itr->second;
        const auto& pack = packs.at(file.pack_hash);

        return boost::make_shared<std::istrstream>(pack.mapped_file_source.data() + file.offset, file.length);
    }
}
