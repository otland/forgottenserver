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
#include "../tools.h"
#include <memory>
#include <unordered_set>

namespace http_api
{

class Peer;
class Router;
using PeerSharedPtr = std::shared_ptr<Peer>; ///Shared pointer to Peer object

class ApiServer : NonCopyable, NonMovable
{
	using Acceptor = asio::ip::tcp::acceptor;
	using Peers = std::unordered_set<PeerSharedPtr>;
	using EndPoint = asio::ip::tcp::endpoint;

	Acceptor acceptor; ///The acceptor used to listen for incomming TCP/IP connections
	Peers peers; ///Set of connections, tracked so that we can perform a clean shutdown
	Strand strand; ///Internal strand for the server to synchronize access to the server
	PeerID peerCounter{}; ///Counter used to generate PeerIDs
	std::unique_ptr<Router> router; ///Router which dispatches handlers

	void accept();
	void start(EndPoint endPoint);
public:
    explicit ApiServer(IoService& service);
	~ApiServer();
	/** \brief Stops the API server
	 *
	 * Stops the API server by closing the acceptor and terminating connections to
	 * all connected \ref Peer objects.
	 * \remark Thread-safe - access to this boject is synchronized with a local \ref ApiServer::strand
	 */
	void stop();

	IoService& getIoService()
	{
		return acceptor.get_io_service();
	}

	void onPeerClose(Peer& peer);
	void loadRoutes();
};

} //namespace http_api
#endif // FS_API_SERVER_SERVER_H
