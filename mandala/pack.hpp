#pragma once

//std
#include <map>
#include <memory>

//mandala
#include "hash.hpp"

//boost
#include <boost\iostreams\device\mapped_file.hpp>

namespace mandala
{
	struct pack_t
	{
		struct file_t
		{
			hash_t pack_hash;
			std::string name;
			uint32_t offset = 0;
			uint32_t length = 0;
			uint32_t crc32 = 0;
		};

        typedef std::map<const hash_t, file_t> files_type;

		pack_t(const std::string& path);
		pack_t(pack_t&& rhs);

        std::string path;
        files_type files;
        boost::iostreams::mapped_file_source mapped_file_source;

		pack_t& operator=(pack_t&& rhs);

	private:

		pack_t(const pack_t&) = delete;
		pack_t& operator=(const pack_t&) = delete;
	};
}
