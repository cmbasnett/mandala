#include "resource.hpp"

namespace mandala
{
	resource_t::resource_t() :
		creation_time(clock_type::now()),
		last_access_time(clock_type::now())
	{
	}
}
