#pragma once

// std
#include <list>
#include <future>

// boost
#include <boost\optional.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\function.hpp>

// naga
#include "http.hpp"

namespace naga
{
    struct HttpResponse;
    struct HttpRequest;

    struct HttpManager
    {
        typedef boost::function<void(boost::shared_ptr<HttpResponse>)> ResponseFunctionType;
		typedef boost::function<void(size_t)> WriteFunctionType;

		boost::shared_ptr<HttpResponse> get(
            std::string url
            //http_headers_type headers = http_headers_type(),
            //http_data_type data = http_data_type(),
			//write_function_type on_write = nullptr
            );

		boost::shared_ptr<HttpRequest> get_async(
			const std::string& url,
            //const http_headers_type& headers = http_headers_type(),
            //const http_data_type& data = http_data_type(),
			ResponseFunctionType on_response = ResponseFunctionType()
			//write_function_type on_write = nullptr
            );

        void tick();

        struct RequestObject
        {
			RequestObject(const boost::shared_ptr<HttpRequest>& request, ResponseFunctionType on_response) :
                request(request),
                on_response(on_response)
            {
            }

            boost::shared_ptr<HttpRequest> request;
			ResponseFunctionType on_response;
        };

    private:
		std::list<RequestObject> request_objects;
		std::mutex request_objects_mutex;
    };

    extern HttpManager http;
}