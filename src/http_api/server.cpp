/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2017  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "otpch.h"

#include "server.h"
#include "peer.h"
#include "router.h"
#include "configmanager.h"

extern ConfigManager g_config;

namespace http_api
{

using IpEndpoint = asio::ip::tcp::endpoint;
using IpAcceptor = asio::ip::tcp::acceptor;
using LocalEndpoint = asio::local::stream_protocol::endpoint;
using LocalAcceptor = asio::local::stream_protocol::acceptor;

namespace detail
{

template<typename Acceptor, typename Endpoint>
void open(Acceptor& acceptor, Endpoint endpoint, ErrorCode& error)
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

const static std::string LOCAL_SOCKET_PREFIX {"unix://"};

class GenericStreamAcceptor
{
public:
	using Pointer = std::unique_ptr<GenericStreamAcceptor>;
	using AcceptHandler = std::function<void(ErrorCode)>;

	static Pointer make(IoService& service, const std::string& address, uint16_t port);

	virtual ~GenericStreamAcceptor() = default;

	virtual void open(const std::string& address, uint16_t port, ErrorCode& error) = 0;
	virtual void close(ErrorCode& error) = 0;
	virtual void asyncAccept(Peer::Socket& socket, AcceptHandler handler) = 0;
};

class LocalAcceptorWrapper : public GenericStreamAcceptor
{
	LocalAcceptor acceptor;

	void open(const std::string& address, uint16_t port, ErrorCode& error) override;
	void close(ErrorCode& error) override;
	void asyncAccept(Peer::Socket& socket, AcceptHandler handler) override;

public:
	LocalAcceptorWrapper(IoService& service);
};

LocalAcceptorWrapper::LocalAcceptorWrapper(IoService& service):
	acceptor{service}
{
}

void LocalAcceptorWrapper::asyncAccept(Peer::Socket& socket, AcceptHandler handler)
{
	acceptor.async_accept(socket, std::move(handler));
}

void LocalAcceptorWrapper::open(const std::string& address, uint16_t /*port*/, ErrorCode& error)
{
	auto pos = address.find(LOCAL_SOCKET_PREFIX);
	//GenericStreamAcceptor::make() already checked if address is longer than the prefix
	auto realAddress = address.substr(pos + LOCAL_SOCKET_PREFIX.size());
	LocalEndpoint endpoint{realAddress};
	::unlink(endpoint.path().c_str());
	detail::open(acceptor, endpoint, error);
}

void LocalAcceptorWrapper::close(ErrorCode& error)
{
	if (!acceptor.is_open()) {
		return;
	}
	auto endpoint = acceptor.local_endpoint();
	acceptor.close(error);
	::unlink(endpoint.path().c_str());
}

class IpAcceptorWrapper : public GenericStreamAcceptor
{
	IpAcceptor acceptor;

	void open(const std::string& address, uint16_t port, ErrorCode& error) override;
	void close(ErrorCode& error) override;
	void asyncAccept(Peer::Socket& socket, AcceptHandler handler) override;

public:
	IpAcceptorWrapper(IoService& service);
};

IpAcceptorWrapper::IpAcceptorWrapper(IoService& service):
	acceptor{service}
{
}

void IpAcceptorWrapper::asyncAccept(Peer::Socket& socket, AcceptHandler handler)
{
	acceptor.async_accept(socket, std::move(handler));
}

void IpAcceptorWrapper::open(const std::string& address, uint16_t port, ErrorCode& error)
{
	IpEndpoint endpoint {asio::ip::address::from_string(address), port};
	detail::open(acceptor, endpoint, error);
}

void IpAcceptorWrapper::close(ErrorCode& error)
{
	if (!acceptor.is_open()) {
		return;
	}
	acceptor.close(error);
}

GenericStreamAcceptor::Pointer GenericStreamAcceptor::make(IoService& service, const std::string& address, uint16_t port)
{
	auto pos = address.find(LOCAL_SOCKET_PREFIX);
	GenericStreamAcceptor::Pointer acceptor;
	if (pos == std::string::npos) {
		acceptor.reset(new IpAcceptorWrapper{service});
	} else if (address.size() > LOCAL_SOCKET_PREFIX.size()) {

		acceptor.reset(new LocalAcceptorWrapper{service});
	} else {
		std::cout << "HTTP API makeAcceptor error: Invalid acceptor type." << std::endl;
	}

	return acceptor;
}


Server::Server(IoService& service) :
	strand(service)
{
	//
}

//We need to default the destructor here because members are incomplete types in the header
Server::~Server() = default;

void Server::accept()
{
	auto peer = std::make_shared<Peer>(*this, *router, peerCounter++);
	acceptor->asyncAccept(peer->socket, strand.wrap([this, peer](ErrorCode err) {
		if (err == asio::error::operation_aborted || err == asio::error::bad_descriptor) {
			std::cout << "HTTP API acceptor closing..." << std::endl;
			return;
		} else if (err) {
			accept();
			std::cerr << "HTTP API acceptor accept() error: " << err.message() << std::endl;
		} else {
			accept();
			peers.emplace(peer);
			peer->onAccept();
		}
	}));
}

void Server::start(const std::string& address, uint16_t port)
{
	strand.dispatch([this, address, port]() {
		ErrorCode err{};
		acceptor = GenericStreamAcceptor::make(getIoService(), address, port);
		acceptor->open(address, port, err);
		if (err) {
			std::cerr << "HTTP API acceptor open() error: " << err.message() << std::endl;
			return;
		}

		accept();
	});
}

void Server::stop()
{
	strand.dispatch([this]() {
		ErrorCode err{};
		acceptor->close(err);
		if (err) {
			std::cerr << "HTTP API acceptor close() error: " << err.message() << std::endl;
		}
		for (const auto& peer : peers) {
			peer->close();
		}
		peers.clear();
	});
}

void Server::onPeerClose(Peer& peer)
{
	auto peerShared = peer.shared_from_this();
	strand.dispatch([this, peerShared]() {
		peers.erase(peerShared);
	});
}

void Server::loadRoutes()
{
	//Dispatcher thread
	router.reset(new Router);
	if (!router->loadRoutingFunctions()) {
		return;
	}
	;
	auto address = g_config.getString(ConfigManager::HTTP_API_IP);
	auto port = static_cast<uint16_t>(g_config.getNumber(ConfigManager::HTTP_API_PORT));
	asio::ip::tcp::endpoint ep{};
	start(address, port);
}

IoService& Server::getIoService()
{
	return strand.get_io_service();
}


} //namespace http_api
