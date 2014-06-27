#pragma once

//std
#include <chrono>

//mandala
#include "hash.hpp"

#include <type_traits>

namespace mandala
{
	using namespace std::chrono;

	struct resource_t
	{
		hash_t hash;
		system_clock::time_point creation_time;
		system_clock::time_point last_access_time;

	protected:
		resource_t();
		resource_t(const resource_t&) = delete;
		resource_t& operator=(const resource_t&) = delete;
	};
};
