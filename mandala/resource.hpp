#pragma once

//std
#include <chrono>

//mandala
#include "hash.hpp"

namespace mandala
{
	struct resource_t
	{
        typedef std::chrono::system_clock clock_type;
        typedef clock_type::time_point time_point_type;

        hash_t hash;
        time_point_type creation_time;
        time_point_type last_access_time;

	protected:
		resource_t();
		resource_t(const resource_t&) = delete;
		resource_t& operator=(const resource_t&) = delete;
	};
}
