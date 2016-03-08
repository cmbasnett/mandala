#pragma once

// std
#include <list>
#include <future>

// boost
#include <boost\optional.hpp>
#include <boost\shared_ptr.hpp>

// naga
#include "http.hpp"

using namespace std;

namespace naga
{
    struct http_response;
    struct http_request;

    struct http_manager
    {
        typedef function<void(boost::shared_ptr<http_response>)> response_callback_type;
        typedef function<void(size_t)> write_callback_type;

        boost::shared_ptr<http_response> get(
            const string& url,
            const http_headers_type& headers = http_headers_type(),
            const http_data_type& data = http_data_type(),
            write_callback_type on_write = nullptr
            );

        void get_async(
            const string& url,
            const http_headers_type& headers = http_headers_type(),
            const http_data_type& data = http_data_type(),
            response_callback_type on_response = nullptr,
            write_callback_type on_write = nullptr
            );

        void tick();

        struct request_object
        {
            request_object(const boost::shared_ptr<http_request>& request, response_callback_type on_response) :
                request(request),
                on_response(on_response)
            {
            }

            boost::shared_ptr<http_request> request;
            response_callback_type on_response;
        };

    private:
        list<request_object> request_objects;
        mutex request_objects_mutex;
    };

    extern http_manager http;
}