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

#include "../otpch.h"

#include "server.h"
#include "peer.h"
#include "router.h"

namespace http_api
{

Server::Server(IoService& service) :
	acceptor(service),
	strand(service)
{
	//
}

Server::~Server() = default;

void Server::accept()
{
	auto peer = std::make_shared<Peer>(*this, *router, peerCounter++);
	acceptor.async_accept(peer->socket, strand.wrap([this, peer](ErrorCode err) {
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

void Server::start(EndPoint endPoint)
{
	strand.dispatch([this, endPoint]() {
		ErrorCode err{};
		acceptor.open(endPoint.protocol(), err);
		if (err) {
			std::cerr << "HTTP API acceptor open() error: " << err.message() << std::endl;
			return;
		}
		asio::socket_base::reuse_address option(true);
		acceptor.set_option(option);
		acceptor.bind(endPoint, err);
		if (err) {
			std::cerr << "HTTP API acceptor bind() error: " << err.message() << std::endl;
			return;
		}
		acceptor.listen(asio::socket_base::max_connections, err);
		if (err) {
			std::cerr << "HTTP API acceptor listen() error: " << err.message() << std::endl;
			return;
		}
		accept();
	});
}

void Server::stop()
{
	strand.dispatch([this]() {
		ErrorCode err{};
		acceptor.close(err);
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
	//TODO: load address and port from config
	asio::ip::tcp::endpoint ep{asio::ip::address_v4(INADDR_ANY), 8080};
	start(ep);
}

} //namespace http_api
