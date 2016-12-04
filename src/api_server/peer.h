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

#ifndef FS_API_SERVER_PEER_H
#define FS_API_SERVER_PEER_H

#include "common.h"
#include "../tools.h"

#include <beast/http.hpp>
namespace http
{

class ApiServer;
class Router;

class Peer : public std::enable_shared_from_this<Peer>, NonCopyable, NonMovable
{
public:
private:
	friend class ApiServer;
	using Socket = asio::ip::tcp::socket;
	using Streambuf = asio::streambuf;
	using Timer = asio::deadline_timer;

	ApiServer& server;
	Router& router;
	Socket socket;
	Timer timer;
	Strand strand;
	Streambuf buffer;
	Request request;
	Response response;
	const PeerID peerID;
	/**The @ref requestCounter variable is used to ensure that the lua environment does not store the responder and
	 * accidentally respond to another request after the current one times out
	 */
	RequestID requestCounter{};
	bool requestKeepAlive{true};


	void onAccept();
	void read();
	void write();
	void internalClose();
	void startTimer();
	void cancelTimer();
public:
	Peer(ApiServer& server, Router& router, PeerID peerID);
	void send(Response response, RequestID requestID);
	void close();
};

using PeerWeakPtr = std::weak_ptr<Peer>;

} //namespace http

#endif // FS_API_SERVER_PEER_H