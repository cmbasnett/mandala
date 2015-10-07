//mandala
#include "resource.hpp"

namespace mandala
{
    resource::resource() :
        creation_time(clock_type::now()),
        last_access_time(clock_type::now())
    {
    }
}
