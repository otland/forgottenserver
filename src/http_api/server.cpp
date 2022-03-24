// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "generic_stream_acceptor.h"
#include "server.h"
#include "peer.h"
#include "router.h"
#include "../configmanager.h"

extern ConfigManager g_config;

namespace HttpApi
{

Server::Server(asio::io_service& service) :
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
		if (acceptor == nullptr) {
			return;
		}

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
		if (!acceptor) {
			return;
		}

		acceptor->close(err);
		if (err) {
			std::cerr << "HTTP API acceptor close() error: " << err.message() << std::endl;
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
	router = std::make_unique<Router>();
	if (!router->loadRoutingFunctions()) {
		return;
	}

	const auto &address = g_config.getString(ConfigManager::HTTP_API_IP);
	auto port = static_cast<uint16_t>(g_config.getNumber(ConfigManager::HTTP_API_PORT));
	start(address, port);
}

asio::io_service& Server::getIoService()
{
	return strand.context();
}


} //namespace HttpApi
