#pragma once

//mandala
#include "hash.h"
#include "pack.h"

namespace mandala
{
	struct pack_mgr_t
	{
		void mount(const std::string& path);
		void unmount_all();

		std::shared_ptr<std::istream> extract(const hash_t& hash) const;
		
	private:
		std::map<hash_t, pack_t::file_t> files;
		std::map<hash_t, pack_t> packs;
	};
};
