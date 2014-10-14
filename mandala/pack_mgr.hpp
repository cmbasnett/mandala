#pragma once

//mandala
#include "hash.hpp"
#include "pack.hpp"

namespace mandala
{
	struct pack_mgr_t
	{
		void mount(const std::string& path);
		void unmount_all();

		std::shared_ptr<std::istream> extract(const hash_t& hash) const;

	private:
		std::map<const hash_t, pack_t::file_t> files;
        std::map<const hash_t, pack_t> packs;
	};
}
