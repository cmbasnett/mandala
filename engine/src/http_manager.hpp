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
    struct http_response;
    struct http_request;

    struct http_manager
    {
        typedef boost::function<void(boost::shared_ptr<http_response>)> response_function_type;
		typedef boost::function<void(size_t)> write_function_type;

        boost::shared_ptr<http_response> get(
            std::string url
            //http_headers_type headers = http_headers_type(),
            //http_data_type data = http_data_type(),
			//write_function_type on_write = nullptr
            );

		boost::shared_ptr<http_request> get_async(
			const std::string& url,
            //const http_headers_type& headers = http_headers_type(),
            //const http_data_type& data = http_data_type(),
			response_function_type on_response = response_function_type()
			//write_function_type on_write = nullptr
            );

        void tick();

        struct request_object
        {
			request_object(const boost::shared_ptr<http_request>& request, response_function_type on_response) :
                request(request),
                on_response(on_response)
            {
            }

            boost::shared_ptr<http_request> request;
			response_function_type on_response;
        };

    private:
        std::list<request_object> request_objects;
		std::mutex request_objects_mutex;
    };

    extern http_manager http;
}