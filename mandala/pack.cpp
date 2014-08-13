//mandala
#include "pack.hpp"

//std
#include <fstream>
#include <sstream>

//boost
#include <boost\iostreams\stream.hpp>
#include <boost\filesystem.hpp>

#define PACK_MAGIC			("PACK")
#define PACK_MAGIC_LENGTH	(4)
#define PACK_VERSION		(1)

namespace mandala
{
	pack_t::pack_t(const std::string& path) :
		path(path)
	{
		auto istream = std::ifstream(path, std::ios_base::binary | std::ios_base::in);

		//magic
        char magic[PACK_MAGIC_LENGTH + 1] = { '\0' };
		istream.read(magic, PACK_MAGIC_LENGTH);

		if (strcmp(PACK_MAGIC, magic) != 0)
		{
			throw std::exception();
		}

		//version
		uint32_t version = 0;
		istream.read(reinterpret_cast<char*>(&version), sizeof(version));

		if (version != PACK_VERSION)
		{
			throw std::exception();
		}

		//file count
		uint32_t file_count = 0;
		istream.read(reinterpret_cast<char*>(&file_count), sizeof(file_count));

		for(uint32_t i = 0; i < file_count; ++i)
		{
			file_t file;

			std::getline(istream, file.name, '\0');

			istream.read(reinterpret_cast<char*>(&file.offset), sizeof(file.offset));
			istream.read(reinterpret_cast<char*>(&file.length), sizeof(file.length));
			istream.read(reinterpret_cast<char*>(&file.crc32), sizeof(file.crc32));

			files.emplace(hash_t(file.name), std::forward<file_t>(file));
		}

		mapped_file_source = boost::iostreams::mapped_file_source(path);
	}

	pack_t::pack_t(pack_t&& copy) :
		path(std::move(copy.path)),
		files(std::move(copy.files)),
		mapped_file_source(std::move(copy.mapped_file_source))
	{
	}

	pack_t& pack_t::operator=(pack_t&& copy)
	{
		path = std::move(copy.path);
		files = std::move(copy.files);
		mapped_file_source = std::move(copy.mapped_file_source);

		return *this;
	}
}
