//mandala
#include "pack_mgr.hpp"
#include "pack.hpp"

//std
#include <strstream>

//boost
#include <boost\filesystem\path.hpp>
#include <boost\iostreams\device\mapped_file.hpp>
#include <boost\iostreams\filtering_streambuf.hpp>
#include <boost\iostreams\stream.hpp>

namespace mandala
{
	void pack_mgr_t::mount(const std::string& path)
	{
		auto pack_hash = hash_t(boost::filesystem::path(path).filename().string());

		packs.erase(pack_hash);
		
		auto packs_itr = packs.insert(std::make_pair(pack_hash, pack_t(path)));

		if (!packs_itr.second)
		{
			throw std::exception();
		}

		auto& pack = packs_itr.first->second;

		for (auto& file : pack.files)
		{
			file.second.pack_hash = pack_hash;

			files[file.first] = file.second;
		}
	}

	void pack_mgr_t::unmount_all()
	{
		files.clear();
		packs.clear();
	}

	std::shared_ptr<std::istream> pack_mgr_t::extract(const hash_t& hash) const
	{
		auto files_itr = files.find(hash);

		if (files_itr == files.end())
		{
            throw std::out_of_range("");
		}

		const auto& file = files_itr->second;
		const auto& pack = packs.at(file.pack_hash);

		return std::make_shared<std::istrstream>(pack.mapped_file_source.data() + file.offset, file.length);
	}
};
