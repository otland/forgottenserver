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

#ifndef FS_HTTP_API_PEER_H_FF52E73D0D1DFC8DCCA08929BACB0739
#define FS_HTTP_API_PEER_H_FF52E73D0D1DFC8DCCA08929BACB0739

#include "tools.h"
#include "common.h"
#include <beast/http.hpp>

namespace http_api
{

class Server;
class Router;

/** \brief Represents a connection to a remote Peer
 *
 * Peer provides all functionality required to handle incoming HTTP requests, dispatch  the request
 * to the \ref Router and allow sending an HTTP response message.
 * \remark Peer objects HAVE to be managed by shared_ptr.
 */
class Peer : public std::enable_shared_from_this<Peer>, NonCopyable, NonMovable
{
public:
	using Socket = asio::generic::stream_protocol::socket;
	using Streambuf = asio::streambuf;
	using Timer = asio::deadline_timer;
private:
	friend class Server;
	/// Reference to the server object. \remark The \ref Peer does not manage its lifetime.
	Server& server;

	/** Reference to the router object which dispatches request handlers.
	  * \remark The \ref Peer does not manage its lifetime.
	  */
	Router& router;
	/// Represents the connection to a remote peer
	Socket socket;

	/// Internal timeout timer
	Timer timer;
	///Used to synchronize access to this object
	Strand strand;

	/// Used to temporarily store incoming HTTP raw data
	Streambuf buffer;

	/// Stores a parsed HTTP request message
	Request request;

	/// Stores an HTTP response message
	Response response;

	/// A number which uniquely identifies a \ref Peer (these numbers are not persistent across process restarts)
	const PeerID peerID;
	/**The \ref requestCounter variable is used to ensure that the lua environment does not store the responder and
	 * accidentally respond to another request after the current one times out
	 */
	RequestID requestCounter{};
	/// Determines whether the next response sent is followed by a connection close
	bool requestKeepAlive{true};


	/** \brief Accept event handler
	 *
	 * This function is called when a remote peer estabilishes a connection.
	 * Enqueues the first read operation and dispatches an event to the router.
	 * \remark Thread-safe - access to this object is synchronized with a local \ref Peer::strand.
	 */
	void onAccept();

	/** \brief Asynchronous read operation enqueuing method
	 *
	 *  Enqueues an asynchronous read operation of an HTTP request. On completion of the read operation
	 *  a handler is dispatched which constructs a Responder and passes it over to the \ref router.
	 *  \remark Thread-unsafe - this function should only be called within the local \ref Peer::strand.
	 *  \remark Only one read operation can be enqueued for a particular socket at any time.
	 **/
	void read();

	/** \brief Asynchronous write operation enqueuing method
	 *
	 * Enqueues an asynchronous write operation of an HTTP response stored in \ref response.
	 * On completion of the write operation a handler is dispatched which closes the connection if
	 * Connection: close HTTP header field is present. Otherwise, a next read operation is enqueued
	 * using \ref read.
	 * \remark Thread-unsafe - this function should only be called within the local \ref Peer::strand.
	 * \remark Only one read operation can be enqueued for a particular socket at any time.
	 */
	void write();

	void internalClose();

	/** \brief Starts the internal timeout timer
	 *
	 * The timer allows detection of a dropped connection in an easy way. The timeout is controlled
	 * by \ref TIMER_TIMEOUT.
	 * \remark Thread-unsafe - this function should only be called within the local \ref Peer::strand.
	 */
	void startTimer();

	/** \brief Stops the internal timeout timer
	 *
	 * Stops the internal timer. Calling this function if the timer is not running is safe.
	 * \remark Thread-unsafe - this function should only be called within the local \ref Peer::strand.
	 */
	void cancelTimer();
public:
	Peer(Server& server, Router& router, PeerID peerID);

	/** \brief Sends an HTTP response for a request
	 *
	 * Sends an HTTP response to this Peer for a request with the provided requestID.
	 * If the request ID is invalid the message is not sent and an error message is printed.
	 * Otherwise the reponse is moved into the Peer object and an asynchronous write operation
	 * is enqueued with \ref Peer::write.
	 * \param[in] response - the HTTP response message
	 * \param[in] requestID - the ID of the request linked to the response
	 * \remark Thread-safe - access to this object is synchronized with a local \ref Peer::strand.
	 */
	void send(Response response, RequestID requestID);

	/** \brief Closes the connection
	 *
	 *  This function closes the connection related to this Peer, cancels all
	 *  enqueued asynchronous operations and removes this Peer from the Peer list
	 *  stored in its parent \ref Peer::server.
	 *  \remark Thread-safe - access to this object is synchronized with a local \ref Peer::strand.
	 *  \remark It is safe to call this method multiple times - subsequent calls will have no effect
	 *  (apart from printing a few warnings).
	 */
	void close();
};

using PeerWeakPtr = std::weak_ptr<Peer>; ///Weak pointer to peer, used to prevent lua from keeping Peers alive for too long

} //namespace http_api

#endif // FS_HTTP_API_PEER_H_FF52E73D0D1DFC8DCCA08929BACB0739
