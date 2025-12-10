#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/error.hpp>
#include <memory>

namespace asio = boost::asio;
namespace beast = boost::beast;

namespace boost::asio {
class io_context;
}

namespace tfs::http {

class Listener final : public std::enable_shared_from_this<Listener>
{
public:
	Listener(asio::io_context& ioc, asio::ip::tcp::acceptor&& acceptor);

	void accept();
	void run() { accept(); }

private:
	void on_accept(beast::error_code ec, asio::ip::tcp::socket socket);

	asio::io_context& ioc;
	asio::ip::tcp::acceptor acceptor;
};

std::shared_ptr<Listener> make_listener(asio::io_context& ioc, asio::ip::tcp::endpoint endpoint);

} // namespace tfs::http
