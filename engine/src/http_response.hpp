#pragma once

// std
#include <string>

#include "http.hpp"

namespace naga
{
    struct http_response
    {
        bool has_error() const { return !exception_ptr; }
        long get_status() const { return status; }
        const std::string& get_content() const { return content; }
        const std::string& get_content_type() const { return content_type; };
        const char* get_error() const
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

    private:
        friend struct http_manager;

        http_status status = http_status::unknown;
        std::string content;
        std::string content_type;
        std::exception_ptr exception_ptr;
    };
}