//naga
#include "package_manager.hpp"
#include "package.hpp"

//std
#include <sstream>
#include <strstream>

//boost
#include <boost\filesystem\path.hpp>
#include <boost\make_shared.hpp>

namespace naga
{
    void PackageManager::mount(const std::string& path)
    {
        std::lock_guard<std::recursive_mutex> lock(mutex);

        const auto package_name = boost::filesystem::path(path).filename().string();

		packages.erase(package_name);

		auto packs_itr = packages.insert(std::make_pair(package_name, Package(path)));

        if (!packs_itr.second)
        {
            std::ostringstream ostringstream;
			ostringstream << "pack \"" << package_name << "\" already mounted";

            throw std::exception(ostringstream.str().c_str());
        }

        auto& pack = packs_itr.first->second;

        for (auto& file : pack.files)
        {
			file.second.package_name = package_name;

            files[file.first] = file.second;
        }
    }

    void PackageManager::unmount_all()
    {
        std::lock_guard<std::recursive_mutex> lock(mutex);

        files.clear();
        packages.clear();
    }

	boost::shared_ptr<std::istream> PackageManager::extract(const std::string& file_name)
    {
        std::lock_guard<std::recursive_mutex> lock(mutex);

		auto files_itr = files.find(file_name);

        if (files_itr == files.end())
        {
            std::ostringstream ostringstream;
			ostringstream << "no such file " << file_name;

            throw std::out_of_range(ostringstream.str().c_str());
        }

        const auto& file = files_itr->second;
        const auto& package = packages.at(file.package_name);

		return boost::make_shared<std::istrstream>(package.mapped_file_source.data() + file.offset, file.length);
    }
}
