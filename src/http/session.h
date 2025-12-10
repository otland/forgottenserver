#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/http/string_body.hpp>
#include <memory>

namespace asio = boost::asio;
namespace beast = boost::beast;

namespace boost::beast::http {
class message_generator;
}

namespace tfs::http {

class Session final : public std::enable_shared_from_this<Session>
{
public:
	Session(asio::ip::tcp::socket&& socket);

	void read();
	void write(beast::http::message_generator&& msg);
	void close();
	void run();

private:
	void on_read(beast::error_code ec, size_t bytes_transferred);
	void on_write(beast::error_code ec, size_t bytes_transferred, bool keep_alive);

	beast::tcp_stream stream;
	beast::flat_buffer buffer;
	beast::http::request<beast::http::string_body> req;
};

std::shared_ptr<Session> make_session(asio::ip::tcp::socket&& socket);

} // namespace tfs::http
