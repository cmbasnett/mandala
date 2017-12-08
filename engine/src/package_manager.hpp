#pragma once

//std
#include <mutex>

//naga
#include "package.hpp"

namespace naga
{
    struct PackageManager
    {
        void mount(const std::string& path);
        void unmount_all();

        boost::shared_ptr<std::istream> extract(const std::string& file_name);

    private:
        std::recursive_mutex mutex;
        std::map<const std::string, Package::File> files;
		std::map<const std::string, Package> packages;
    };
}
