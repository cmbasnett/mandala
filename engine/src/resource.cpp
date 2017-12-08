//naga
#include "resource.hpp"

namespace naga
{
	Resource::Resource() :
        creation_time(ClockType::now()),
		last_access_time(ClockType::now())
    {
    }
}
