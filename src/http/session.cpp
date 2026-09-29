#include "session.h"

#include "router.h"

#include <boost/asio/dispatch.hpp>
#include <boost/asio/read.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/write.hpp>
#include <fmt/core.h>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace proxy_protocol = tfs::net::proxy_protocol;

namespace tfs::http {

Session::Session(asio::ip::tcp::socket&& socket) : stream{std::move(socket)} {}

void Session::read()
{
	using namespace std::chrono_literals;

	// Make the request empty before reading,
	// otherwise the operation behavior is undefined.
	req = {};

	// Set the timeout.
	stream.expires_after(30s);

	// Read a request
	async_read(stream, buffer, req, [self = shared_from_this()](beast::error_code ec, size_t bytes_transferred) {
		self->on_read(ec, bytes_transferred);
	});
}

void Session::write(beast::http::message_generator&& msg)
{
	bool keep_alive = msg.keep_alive();

	// Write the response
	async_write(stream, std::move(msg),
	            [self = shared_from_this(), keep_alive](beast::error_code ec, size_t bytes_transferred) {
		            self->on_write(ec, bytes_transferred, keep_alive);
	            });
}

void Session::close()
{
	// Send a TCP shutdown
	beast::error_code ec;
	stream.socket().shutdown(asio::ip::tcp::socket::shutdown_both, ec);

	// At this point the connection is closed gracefully
}

void Session::run()
{
	// We need to be executing within a strand to perform async operations
	// on the I/O objects in this session. Although not strictly necessary
	// for single-threaded contexts, this example code is written to be
	// thread-safe by default.
	dispatch(stream.get_executor(), [self = shared_from_this()] { self->start(); });
}

void Session::start()
{
	beast::error_code ec;
	auto endpoint = stream.socket().remote_endpoint(ec);
	if (ec) {
		fmt::print(stderr, "{}: {}\n", __FUNCTION__, ec.message());
		return;
	}

	ip = endpoint.address().to_string();

	// Only a proxy running on the same host is trusted to announce the original client address
	if (proxy_protocol::isTrustedPeer(endpoint.address())) {
		read_proxy_header();
	} else {
		read();
	}
}

void Session::read_proxy_header()
{
	using namespace std::chrono_literals;

	stream.expires_after(30s);

	// Read a full fixed-size header before deciding: any HTTP request is longer than that, so a non-proxied client
	// is not stalled by this read
	asio::async_read(stream, buffer.prepare(proxy_protocol::HEADER_LENGTH),
	                 [self = shared_from_this()](beast::error_code ec, size_t bytes_transferred) {
		                 self->on_read_proxy_header(ec, bytes_transferred);
	                 });
}

void Session::on_read_proxy_header(beast::error_code ec, size_t bytes_transferred)
{
	buffer.commit(bytes_transferred);

	if (ec == asio::error::eof) {
		close();
		return;
	}

	if (ec) {
		fmt::print(stderr, "{}: {}\n", __FUNCTION__, ec.message());
		return;
	}

	auto data = static_cast<const uint8_t*>(buffer.data().data());
	if (!proxy_protocol::matchesSignature(data, proxy_protocol::SIGNATURE.size())) {
		// Not relayed by a proxy, what has been read is the start of the HTTP request
		read();
		return;
	}

	auto header = proxy_protocol::parseHeader(data);
	if (!header) {
		fmt::print(stderr, "{}: malformed PROXY protocol header from {}\n", __FUNCTION__, ip);
		return;
	}

	proxyHeader = *header;
	buffer.consume(proxy_protocol::HEADER_LENGTH);

	if (proxyHeader.length == 0) {
		apply_proxy_header();
		return;
	}

	using namespace std::chrono_literals;

	stream.expires_after(30s);

	// Read the address block and any TLVs following it
	asio::async_read(stream, buffer.prepare(proxyHeader.length),
	                 [self = shared_from_this()](beast::error_code ec, size_t bytes_transferred) {
		                 self->on_read_proxy_address(ec, bytes_transferred);
	                 });
}

void Session::on_read_proxy_address(beast::error_code ec, size_t bytes_transferred)
{
	buffer.commit(bytes_transferred);

	if (ec == asio::error::eof) {
		close();
		return;
	}

	if (ec) {
		fmt::print(stderr, "{}: {}\n", __FUNCTION__, ec.message());
		return;
	}

	apply_proxy_header();
}

void Session::apply_proxy_header()
{
	// A LOCAL command (e.g. a health check) is the proxy connecting on its own behalf, the real socket address
	// applies and the connection is not treated as relayed
	if (proxyHeader.command == proxy_protocol::Command::PROXY) {
		auto data = static_cast<const uint8_t*>(buffer.data().data());
		if (auto address = proxy_protocol::parseSourceAddress(proxyHeader, data)) {
			ip = address->to_string();
		}
		proxied = true;
	}

	buffer.consume(proxyHeader.length);
	read();
}

void Session::on_read(beast::error_code ec, size_t /*bytes_transferred*/)
{
	if (ec == beast::http::error::end_of_stream) {
		close();
		return;
	}

	if (ec) {
		fmt::print(stderr, "{}: {}\n", __FUNCTION__, ec.message());
		return;
	};

	write(handle_request(std::move(req), ip, proxied));
}

void Session::on_write(beast::error_code ec, size_t /*bytes_transferred*/, bool keep_alive)
{
	if (ec) {
		fmt::print(stderr, "{}: {}\n", __FUNCTION__, ec.message());
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

std::shared_ptr<Session> make_session(asio::ip::tcp::socket&& socket)
{
	return std::make_shared<Session>(std::move(socket));
}

} // namespace tfs::http
