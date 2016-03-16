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
		const std::string& get_url() const { return url; }
		const http_method get_method() const { return method; }
		const http_headers_type& get_headers() const { return headers; }
		const http_data_type& get_data() const { return data; }

	private:
		friend struct http_manager;

        std::string url;
		http_method method = http_method::get;
        http_headers_type headers;
        http_data_type data;
        std::future<boost::shared_ptr<http_response>> response;
    };
}