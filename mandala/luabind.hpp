#pragma once

//std
#include <memory>

namespace luabind
{
    namespace detail
    {
        namespace has_get_pointer_
        {
            template<typename T>
            T* get_pointer(std::shared_ptr<T> const& p)
            {
                return p.get();
            }
        }
    }
}
