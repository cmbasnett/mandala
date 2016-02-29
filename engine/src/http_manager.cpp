//std
#include <thread>
#include <sstream>
#include <map>

//boost
#include <boost/algorithm/string.hpp>

//curl
#include <curl\curl.h>

//naga
#include "http_manager.hpp"

namespace naga
{
    http_manager http;

    size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata)
    {
        auto stream = static_cast<std::ostringstream*>(userdata);
        auto count = size * nmemb;
        stream->write(ptr, count);
        return count;
    }

    void http_manager::get(const headers_type& headers, const std::string& url, std::function<void(long, std::string)> callback)
    {
        auto thread = std::thread([&]()
        {
            std::ostringstream stream;
            long response_code = 404;
            const char* content_type = nullptr;
            curl_slist* headers_list = nullptr;

            for (const auto& header : headers)
            {
                auto header_line = header.first + ": " + header.second;

                curl_slist_append(headers_list, header_line.c_str());
            }

            auto curl = curl_easy_init();

            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream);

            auto code = curl_easy_perform(curl);

            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
            curl_slist_free_all(headers_list);
            curl_easy_cleanup(curl);

            if (callback)
            {
                callback(response_code, stream.str());
            }
        });

        thread.detach();
    }
}