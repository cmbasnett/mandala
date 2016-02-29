#pragma once

// std
#include <map>

namespace naga
{
    struct http_request;

    struct http_manager
    {
        typedef std::map<std::string, std::string> headers_type;

        void get(const headers_type& headers, const std::string& url, std::function<void(long, std::string)> callback);

    private:
    };

    extern http_manager http;
}