#pragma once

//mandala
#include "types.hpp"

//std
#include <string>
#include <thread>
#include <functional>
#include <list>

namespace mandala
{
	struct http_mgr_t
	{
		struct request_t
		{
			typedef std::function<void(const std::string&)> callback_type;

			request_t(const std::string& url, callback_type callback);

			std::thread thread;
			size_t bytes_read = 0;
			callback_type _callback;
		};

		http_mgr_t() = default;

		std::list<std::shared_ptr<request_t>> requests;

		void get(const std::string& url);
	};

	extern http_mgr_t http;
}
