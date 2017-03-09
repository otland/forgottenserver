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

#include "generic_stream_acceptor.h"
#include "server.h"
#include "peer.h"
#include "router.h"
#include "configmanager.h"

extern ConfigManager g_config;

namespace HttpApi
{

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


} //namespace HttpApi
