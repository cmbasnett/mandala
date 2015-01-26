//std
#include <thread>
#include <fstream>

//boost
#include <boost\network\protocol\http\client.hpp>
#include <boost\uuid\uuid_generators.hpp>

//mandala
#include "http_mgr.hpp"
#include "platform.hpp"

namespace mandala
{
	http_mgr_t http;

	using namespace boost::network;

	struct body_handler {
		explicit body_handler() { body = ""; }

		BOOST_NETWORK_HTTP_BODY_CALLBACK(operator(), range, error) {
			// in here, range is the Boost.Range iterator_range, and error is
			// the Boost.System error code.
			if (!error) {
				std::cout << body.size() << ",..." << std::endl;
				body.append(boost::begin(range), boost::end(range));
				std::cout << "...," << body.size() << std::endl;
			}
			else {
				if (error == boost::asio::error::eof) {
					static boost::uuids::random_generator uuid_gen = boost::uuids::random_generator();
					std::string id = ""/*boost::lexical_cast<std::string>(uuid_gen())*/;
					id = id + ".jpg";

					std::ofstream out_file;
					out_file.open(id, std::ios::out | std::ios::binary);
					out_file << body;
					std::cout << "Wrote file!" << std::endl;
				}
				std::cout << "ERROR OCCURRED: " << error << std::endl;
			}
		}

		std::string body;
	};

	http_mgr_t::request_t::request_t(const std::string& url, callback_type callback) :
		_callback(callback)
	{
		http::client::options options;
		options.follow_redirects(true);

		http::client client(options);

		http::client::request request(url);
		auto response = client.get(request, body_handler());
	}

	void http_mgr_t::get(const std::string& url)
	{
		auto request = std::make_shared<request_t>(url, [](const std::string& response)
		{
			std::cout << response;
		});

		requests.emplace_back(request);
	}
}