#pragma once

#include "../proxyprotocol.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/http/message_generator.hpp>
#include <boost/beast/http/string_body.hpp>
#include <memory>
#include <string>

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
	void start();
	void on_read(boost::beast::error_code ec, size_t bytes_transferred);
	void on_write(boost::beast::error_code ec, size_t bytes_transferred, bool keep_alive);

	void read_proxy_header();
	void on_read_proxy_header(boost::beast::error_code ec, size_t bytes_transferred);
	void on_read_proxy_address(boost::beast::error_code ec, size_t bytes_transferred);
	void apply_proxy_header();

	boost::beast::tcp_stream stream;
	boost::beast::flat_buffer buffer;
	boost::beast::http::request<boost::beast::http::string_body> req;

	std::string ip;
	tfs::net::proxy_protocol::Header proxyHeader{};
	bool proxied = false;
};

std::shared_ptr<Session> make_session(boost::asio::ip::tcp::socket&& socket);

} // namespace tfs::http
