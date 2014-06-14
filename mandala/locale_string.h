#pragma once

//std
#include <string>

//mandala
#include "resource.hpp"

namespace mandala
{
    struct locale_string_t : std::wstring, resource_t
    {
    };
};
