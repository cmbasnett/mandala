#pragma once

namespace mandala
{
    struct http_request_t
    {
		http_request_t(const std::string& host, const std::string& path);

		std::string host;
		std::string path;

        void send();
    };
}
