// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "generic_stream_acceptor.h"

#include <memory>

namespace Http
{

namespace
{

namespace detail
{

template<typename Acceptor, typename Endpoint>
static void open(Acceptor& acceptor, Endpoint endpoint, ErrorCode& error)
{
	acceptor.open(endpoint.protocol(), error);
	if (error) {
		return;
	}

	asio::socket_base::reuse_address option(true);
	acceptor.set_option(option, error);
	if (error) {
		return;
	}

	acceptor.bind(endpoint, error);
	if (error) {
		return;
	}

	acceptor.listen(asio::socket_base::max_connections, error);
	if (error) {
		return;
	}
}

} //namespace detail

static const std::string LOCAL_SOCKET_PREFIX {"unix://"};

#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
class LocalAcceptorWrapper : public GenericStreamAcceptor
{
public:
	explicit LocalAcceptorWrapper(asio::io_service& service):
		acceptor{service}
	{
	}

private:
	using LocalEndpoint = asio::local::stream_protocol::endpoint;
	using LocalAcceptor = asio::local::stream_protocol::acceptor;

	LocalAcceptor acceptor;

	void open(const std::string& address, uint16_t port, ErrorCode& error) override
	{
		auto prefixPosition = address.find(LOCAL_SOCKET_PREFIX);
		//GenericStreamAcceptor::make() already checked if address is longer than the prefix
		auto path = address.substr(prefixPosition + LOCAL_SOCKET_PREFIX.size());
		LocalEndpoint endpoint{path};
		::unlink(endpoint.path().c_str());
		detail::open(acceptor, endpoint, error);
	}

	void close(ErrorCode& error) override
	{
		if (!acceptor.is_open()) {
			return;
		}
		auto endpoint = acceptor.local_endpoint();
		acceptor.close(error);
		::unlink(endpoint.path().c_str());
	}

	void asyncAccept(Peer::Socket& socket, AcceptHandler handler) override
	{
		acceptor.async_accept(socket, std::move(handler));
	}
};
#endif //defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)

class IpAcceptorWrapper : public GenericStreamAcceptor
{
public:
	explicit IpAcceptorWrapper(asio::io_service& service):
		acceptor{service}
	{
	}

private:
	using IpEndpoint = asio::ip::tcp::endpoint;
	using IpAcceptor = asio::ip::tcp::acceptor;

	IpAcceptor acceptor;

	void open(const std::string& address, uint16_t port, ErrorCode& error) override
	{
		IpEndpoint endpoint {asio::ip::address::from_string(address), port};
		detail::open(acceptor, endpoint, error);
	}

	void close(ErrorCode& error) override
	{
		if (!acceptor.is_open()) {
			return;
		}
		acceptor.close(error);
	}

	void asyncAccept(Peer::Socket& socket, AcceptHandler handler) override
	{
		acceptor.async_accept(socket, std::move(handler));
	}

};

}

GenericStreamAcceptor::Pointer GenericStreamAcceptor::make(asio::io_service& service, const std::string& address, uint16_t port)
{
	auto pos = address.find(LOCAL_SOCKET_PREFIX);
	GenericStreamAcceptor::Pointer acceptor;
	if (pos == std::string::npos) {
		acceptor = std::make_unique<IpAcceptorWrapper>(service);
	} else if (address.size() > LOCAL_SOCKET_PREFIX.size()) {
#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
		acceptor = std::make_unique<LocalAcceptorWrapper>(service);
#else
		std::cerr << "HTTP API makeAcceptor error: Local stream sockets not supported" << std::endl;
#endif //defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
	} else {
		std::cerr << "HTTP API makeAcceptor error: Invalid acceptor type" << std::endl;
	}

	return acceptor;
}

} //namespace Http
