//std
#include <thread>
#include <sstream>
#include <map>

//boost
#include <boost\make_shared.hpp>

//curl
#include <curl\curl.h>

//naga
#include "http_manager.hpp"
#include "http_response.hpp"
#include "http_request.hpp"

namespace naga
{
    http_manager http;

	size_t write_function(char* ptr, size_t size, size_t nmemb, void* userdata)
	{
		auto stream = static_cast<std::ostringstream*>(userdata);
		size_t count = size * nmemb;

		stream->write(ptr, count);

		//if (on_write)
		//{
		//	on_write(nmemb);
		//}

		return count;
	}

	boost::shared_ptr<http_response> http_manager::get(std::string url/*, http_headers_type headers, http_data_type data, write_function_type on_write*/)
    {
		std::ostringstream stream;
        long response_code = 404;
        const char* content_type = nullptr;
        curl_slist* headers_list = nullptr;
		f64 elapsed = 0.0;
        char error_buffer[CURL_ERROR_SIZE];

        memset(error_buffer, '\0', sizeof(error_buffer));

        //for (const auto& header : headers)
        //{
        //    auto header_line = header.first + ": " + header.second;

        //    curl_slist_append(headers_list, header_line.c_str());
        //}

        auto curl = curl_easy_init();

        assert(curl);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_function);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream);

        if (curl_easy_perform(curl) != CURLE_OK)
        {
            throw std::exception(error_buffer);
        }

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
		curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &elapsed);
        curl_slist_free_all(headers_list);
        curl_easy_cleanup(curl);

        auto response = boost::make_shared<http_response>();
        response->content = stream.str();
        response->content_type = content_type;
        response->status = static_cast<http_status>(response_code);
		response->elapsed = elapsed;
        //TODO: link request

        return response;
    }

    boost::shared_ptr<http_request> http_manager::get_async(
        const std::string& url,
        //const http_headers_type& headers,
        //const http_data_type& data,
        response_function_type on_response
		//write_function_type on_write
		)
    {
        auto request = boost::make_shared<http_request>();
        request->method = http_method::GET;
        request->url = url;
        //request->headers = headers;
        //request->data = data;
        request->response = async(&http_manager::get, this, url/*, headers, data, on_write*/);

		std::lock_guard<std::mutex> lock(request_objects_mutex);
        request_objects.emplace_back(request, on_response);

		return request;
    }

    void http_manager::tick()
    {
		std::lock_guard<std::mutex> lock(request_objects_mutex);

        auto itr = request_objects.begin();

        while (itr != request_objects.end())
        {
            auto& request_object = *itr;
            auto& request = itr->request;

            boost::shared_ptr<http_response> response;

            if (request->response.valid())
            {
                try
                {
                    response = request->response.get();
                }
                catch (...)
                {
                    // store exception in response
                    response = boost::make_shared<http_response>();
                    response->exception_ptr = std::current_exception();
                }

                if (request_object.on_response)
                {
                    request_object.on_response(response);
                }

                itr = request_objects.erase(itr);

                continue;
            }

            ++itr;
        }
    }
}