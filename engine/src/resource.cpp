//naga
#include "resource.hpp"

namespace naga
{
    resource::resource() :
        creation_time(clock_type::now()),
        last_access_time(clock_type::now())
    {
    }
}
