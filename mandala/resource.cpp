#include "resource.hpp"

namespace mandala
{
	resource_t::resource_t() :
		creation_time(std::chrono::system_clock::now()),
		last_access_time(std::chrono::system_clock::now())
	{
	}
}
