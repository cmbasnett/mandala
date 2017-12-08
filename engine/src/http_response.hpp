#pragma once

// std
#include <string>

#include "types.hpp"
#include "http.hpp"

namespace naga
{
    struct HttpResponse
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
		const f64 get_elapsed() const { return elapsed; }

    private:
        friend struct HttpManager;

		HttpStatus status = HttpStatus::UNKNOWN;
        std::string content;
		std::string content_type;
		f64 elapsed = 0.0;
        std::exception_ptr exception_ptr;
    };
}