#include "listener.h"

#include "fail.h"
#include "session.h"

#include <boost/asio/strand.hpp>

namespace tfs::http {

Listener::Listener(boost::asio::io_context& ioc, boost::asio::ip::tcp::acceptor&& acceptor) :
    ioc{ioc}, acceptor{std::move(acceptor)}
{}

void Listener::accept()
{
	// The new connection gets its own strand
	acceptor.async_accept(
	    boost::asio::make_strand(ioc),
	    [self = shared_from_this()](boost::beast::error_code ec, boost::asio::ip::tcp::socket socket) {
		    self->on_accept(ec, std::move(socket));
	    });
}

void Listener::on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket)
{
	if (ec) {
		fail(ec, __FUNCTION__);
		return;
	}

	// Create the session and run it
	auto session = make_session(std::move(socket));
	session->run();

	// Accept another connection
	accept();
}

std::shared_ptr<Listener> make_listener(boost::asio::io_context& ioc, boost::asio::ip::tcp::endpoint endpoint)
{
	boost::asio::ip::tcp::acceptor acceptor{boost::asio::make_strand(ioc)};

	boost::beast::error_code ec;
	if (acceptor.open(endpoint.protocol(), ec); ec) {
		throw std::runtime_error(ec.message());
	}
	if (acceptor.set_option(boost::asio::socket_base::reuse_address(true), ec); ec) {
		throw std::runtime_error(ec.message());
	}
	if (acceptor.bind(endpoint, ec); ec) {
		throw std::runtime_error(ec.message());
	}
	if (acceptor.listen(boost::asio::socket_base::max_listen_connections, ec); ec) {
		throw std::runtime_error(ec.message());
	}

	return std::make_shared<Listener>(ioc, std::move(acceptor));
}

} // namespace tfs::http
