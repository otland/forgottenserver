// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_HTTP_API_SERVER_H
#define FS_HTTP_API_SERVER_H

#include "common.h"
#include "../tools.h"
#include <memory>
#include <unordered_set>

namespace HttpApi
{

class Peer;
class Router;
using PeerSharedPtr = std::shared_ptr<Peer>; ///Shared pointer to Peer object

class GenericStreamAcceptor;

/** \brief represents the HTTP API subsystem
 *
 * This class handles preparation of the API subsystem and handling of incoming connections.
 * \remark Thread-safe - access to this object is synchronized with a local \ref Server::strand
 */
class Server : NonCopyable, NonMovable
{
	using Acceptor = GenericStreamAcceptor;
	using Peers = std::unordered_set<PeerSharedPtr>;

	std::unique_ptr<Acceptor> acceptor; ///The acceptor used to listen for incomming TCP/IP connections
	Peers peers; ///Set of connections, tracked so that we can perform a clean shutdown
	Strand strand; ///Internal strand for the server to synchronize access to the server
	PeerId peerCounter{}; ///Counter used to generate PeerIds
	std::unique_ptr<Router> router; ///Router which dispatches handlers

	void accept();
	void start(const std::string& address, uint16_t port);
public:

    explicit Server(IoService& service);
	~Server();

	/** \brief Stops the API server
	 *
	 * Stops the API server by closing the acceptor and terminating connections to
	 * all connected \ref Peer objects.
	 * \remark Thread-safe - access to this object is synchronized with a local \ref Server::strand
	 */
	void stop();

	IoService& getIoService();

	/** \brief Handles closing of a remote connection
	 *
	 * Handles removing memory related to the \ref Peer that has just disconnected.
	 * \remark Thread-safe - access to this object is synchronized with a local \ref Server::strand
	 */
	void onPeerClose(Peer& peer);

	/** \brief loads routing lua subsystem
	 *
	 * Loads the routing lua subsystem. If failure occurs the server will continue to run but the API
	 * will be disabled.
	 * \remark NOT Thread-safe - access to this object is NOT synchronized
	 */
	void loadRoutes();
};

} //namespace HttpApi

#endif // FS_HTTP_API_SERVER_H
