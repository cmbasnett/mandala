//std
#include <iostream>
#include <thread>

//boost
#include <boost\asio.hpp>

//mandala
#include "http_request.hpp"

namespace mandala
{
    http_request_t::http_request_t(const std::string& host, const std::string& path) :
        host(host),
        path(path)
    {
    }

    void http_request_t::send()
    {
        using namespace boost::asio;

        io_service io_service;
        ip::tcp::resolver resolver(io_service);
        ip::tcp::resolver::query query(host, "http");

        auto endpoint_itr = resolver.resolve(query);
        ip::tcp::resolver::iterator end;

        ip::tcp::socket socket(io_service);

        boost::system::error_code error = boost::asio::error::host_not_found;

        while (endpoint_itr != end)
        {
            socket.close();
            socket.connect(*endpoint_itr++, error);
        }

        if (error)
        {
            throw boost::system::system_error(error);
        }

        boost::asio::streambuf request;
        std::ostream request_stream(&request);
        request_stream << "GET " << path << " HTTP/1.0\r\n";
        request_stream << "Host: " << host << "\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Connection: close\r\n";
        request_stream << "\r\n";

        boost::asio::write(socket, request);

        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\r\n");

        std::istream response_stream(&response);

        //http version
        std::string http_version;
        response_stream >> http_version;

        if (!response_stream || http_version.substr(0, 5) != "HTTP/")
        {
            std::cout << "Invalid response\n";
        }

        //status code
        uint32_t status_code;
        response_stream >> status_code;

        if (status_code != 200)
        {
            std::cout << "Response returned with status code " << status_code << "\n";
        }

        //status message
        std::string status_message;
        std::getline(response_stream, status_message);

        // Read the response headers, which are terminated by a blank line.
        boost::asio::read_until(socket, response, "\r\n\r\n");

        std::ostringstream oss;

        // Write whatever content we already have to output.
        if (response.size() > 0)
        {
            oss << &response;
        }

        while (read(socket, response, transfer_at_least(1), error))
        {
            oss << &response;
        }

        if (error != boost::asio::error::eof)
        {
            throw boost::system::system_error(error);
        }
    }
}
