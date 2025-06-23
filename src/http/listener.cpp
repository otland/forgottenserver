#include "listener.h"

#include "session.h"

#include <boost/asio/strand.hpp>
#include <print>

namespace asio = boost::asio;
namespace beast = boost::beast;

namespace tfs::http {

Listener::Listener(asio::io_context& ioc, asio::ip::tcp::acceptor&& acceptor) : ioc{ioc}, acceptor{std::move(acceptor)}
{}

void Listener::accept()
{
	// The new connection gets its own strand
	acceptor.async_accept(asio::make_strand(ioc),
	                      [self = shared_from_this()](beast::error_code ec, asio::ip::tcp::socket socket) {
		                      self->on_accept(ec, std::move(socket));
	                      });
}

void Listener::on_accept(beast::error_code ec, asio::ip::tcp::socket socket)
{
	if (ec) {
		std::println(stderr, "{}: {}", __FUNCTION__, ec.message());
		return;
	}

	// Create the session and run it
	auto session = make_session(std::move(socket));
	session->run();

	// Accept another connection
	accept();
}

std::shared_ptr<Listener> make_listener(asio::io_context& iocontext, asio::ip::tcp::endpoint endpoint)
{
	asio::ip::tcp::acceptor acceptor{asio::make_strand(iocontext)};

	beast::error_code ec;
	if (acceptor.open(endpoint.protocol(), ec); ec) {
		throw std::runtime_error(ec.message());
	}
	if (acceptor.set_option(asio::socket_base::reuse_address(true), ec); ec) {
		throw std::runtime_error(ec.message());
	}
	if (acceptor.bind(endpoint, ec); ec) {
		throw std::runtime_error(ec.message());
	}
	if (acceptor.listen(asio::socket_base::max_listen_connections, ec); ec) {
		throw std::runtime_error(ec.message());
	}

	return std::make_shared<Listener>(iocontext, std::move(acceptor));
}

} // namespace tfs::http
