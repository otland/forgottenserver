#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/http/message_generator.hpp>
#include <boost/beast/http/string_body.hpp>
#include <memory>

namespace tfs::http {

class Session final : public std::enable_shared_from_this<Session>
{
public:
	Session(boost::asio::ip::tcp::socket&& socket);

	void read();
	void write(boost::beast::http::message_generator&& msg);
	void close();
	void run();

private:
	void on_read(boost::beast::error_code ec, size_t bytes_transferred);
	void on_write(boost::beast::error_code ec, size_t bytes_transferred, bool keep_alive);

	boost::beast::tcp_stream stream;
	boost::beast::flat_buffer buffer;
	boost::beast::http::request<boost::beast::http::string_body> req;
};

std::shared_ptr<Session> make_session(boost::asio::ip::tcp::socket&& socket);

} // namespace tfs::http
