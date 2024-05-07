#include "session.h"

#include "fail.h"
#include "router.h"

#include <boost/asio/dispatch.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/write.hpp>

namespace tfs::http {

Session::Session(boost::asio::ip::tcp::socket&& socket) : stream{std::move(socket)} {}

void Session::read()
{
	using namespace std::chrono_literals;

	// Make the request empty before reading,
	// otherwise the operation behavior is undefined.
	req = {};

	// Set the timeout.
	stream.expires_after(30s);

	// Read a request
	boost::beast::http::async_read(stream, buffer, req,
	                               [self = shared_from_this()](boost::beast::error_code ec, size_t bytes_transferred) {
		                               self->on_read(ec, bytes_transferred);
	                               });
}

void Session::write(boost::beast::http::message_generator&& msg)
{
	bool keep_alive = msg.keep_alive();

	// Write the response
	boost::beast::async_write(
	    stream, std::move(msg),
	    [self = shared_from_this(), keep_alive](boost::beast::error_code ec, size_t bytes_transferred) {
		    self->on_write(ec, bytes_transferred, keep_alive);
	    });
}

void Session::close()
{
	// Send a TCP shutdown
	boost::beast::error_code ec;
	stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

	// At this point the connection is closed gracefully
}

void Session::run()
{
	// We need to be executing within a strand to perform async operations
	// on the I/O objects in this session. Although not strictly necessary
	// for single-threaded contexts, this example code is written to be
	// thread-safe by default.
	boost::asio::dispatch(stream.get_executor(), [self = shared_from_this()] { self->read(); });
}

void Session::on_read(boost::beast::error_code ec, size_t /*bytes_transferred*/)
{
	if (ec == boost::beast::http::error::end_of_stream) {
		close();
		return;
	}

	if (ec) {
		fail(ec, __FUNCTION__);
		return;
	};

	auto ip = stream.socket().remote_endpoint().address().to_string();
	write(handle_request(std::move(req), ip));
}

void Session::on_write(boost::beast::error_code ec, size_t /*bytes_transferred*/, bool keep_alive)
{
	if (ec) {
		fail(ec, __FUNCTION__);
		return;
	};

	if (!keep_alive) {
		// This means we should close the connection, usually because
		// the response indicated the "Connection: close" semantic.
		close();
		return;
	}

	read();
}

std::shared_ptr<Session> make_session(boost::asio::ip::tcp::socket&& socket)
{
	return std::make_shared<Session>(std::move(socket));
}

} // namespace tfs::http
