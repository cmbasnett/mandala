#pragma once

// std
#include <future>

// boost
#include <boost\shared_ptr.hpp>

// naga
#include "http.hpp"

namespace naga
{
    struct HttpResponse;

    struct HttpRequest
    {
		const std::string& get_url() const { return url; }
		const HttpMethod get_method() const { return method; }
		const HttpHeadersType& get_headers() const { return headers; }
		const HttpDataType& get_data() const { return data; }

	private:
		friend struct HttpManager;

        std::string url;
		HttpMethod method = HttpMethod::GET;
		HttpHeadersType headers;
		HttpDataType data;
        std::future<boost::shared_ptr<HttpResponse>> response;
    };
}