#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/error.hpp>
#include <memory>

namespace tfs::http {

class Listener final : public std::enable_shared_from_this<Listener>
{
public:
	Listener(boost::asio::io_context& ioc, boost::asio::ip::tcp::acceptor&& acceptor);

	void accept();
	void run() { accept(); }

private:
	void on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket);

	boost::asio::io_context& ioc;
	boost::asio::ip::tcp::acceptor acceptor;
};

std::shared_ptr<Listener> make_listener(boost::asio::io_context& ioc, boost::asio::ip::tcp::endpoint endpoint);

} // namespace tfs::http
