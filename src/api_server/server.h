/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2016  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_API_SERVER_SERVER_H
#define FS_API_SERVER_SERVER_H

#include "common.h"
#include "router.h"
#include <memory>
#include <unordered_set>
#include <boost/asio.hpp>
#include "../tools.h"

namespace http
{

class Peer;
using PeerSharedPtr = std::shared_ptr<Peer>;

class ApiServer : NonCopyable, NonMovable
{
	using Acceptor = asio::ip::tcp::acceptor;
	using Peers = std::unordered_set<PeerSharedPtr>;
	using EndPoint = asio::ip::tcp::endpoint;

	Acceptor acceptor;
	Peers peers;
	Strand strand;
	PeerID peerCounter{};
	std::unique_ptr<Router> router;
	void accept();
public:
    explicit ApiServer(IoService& service);
	void start(EndPoint endPoint);
	void stop();

	IoService& getIoService() {
		return acceptor.get_io_service();
	}

	void onPeerClose(Peer& peer);
	void loadRoutes();
};

} //namespace http
#endif // FS_API_SERVER_SERVER_H
