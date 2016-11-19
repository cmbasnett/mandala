#pragma once

#include "http_response.hpp"

namespace naga
{
    const char* http_response::get_error() const
    {
        try
        {
            if (exception_ptr)
            {
                std::rethrow_exception(exception_ptr);
            }
        }
        catch (const std::exception& e)
        {
            return e.what();
        }

        return "";
    }
}