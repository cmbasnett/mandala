#pragma once

// std
#include <future>

// boost
#include <boost\shared_ptr.hpp>

// naga
#include "http.hpp"

namespace naga
{
    struct http_response;

    struct http_request
    {
        std::string url;
        http_method method;
        http_headers_type headers;
        http_data_type data;
        std::future<boost::shared_ptr<http_response>> response;
    };
}