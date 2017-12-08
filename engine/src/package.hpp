#pragma once

//std
#include <map>

//boost
#include <boost\shared_ptr.hpp>
#include <boost\iostreams\device\mapped_file.hpp>

//naga
#include "types.hpp"

namespace naga
{
    struct Package
    {
        struct File
        {
			std::string package_name;
            std::string name;
            u32 offset = 0;
            u32 length = 0;
            u32 crc32 = 0;
        };

		typedef std::map<const std::string, File> FilesType;

		Package(const std::string& path);
		Package(Package&& rhs);

        std::string path;
        FilesType files;
        boost::iostreams::mapped_file_source mapped_file_source;

		Package& operator=(Package&& rhs);

    private:

		Package(const Package&) = delete;
		Package& operator=(const Package&) = delete;
    };
}
