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

#ifndef FS_HTTP_API_RESPONDER_H_8F52C86127F133F8E606D96A33069264
#define FS_HTTP_API_RESPONDER_H_8F52C86127F133F8E606D96A33069264

#include "tools.h"
#include "common.h"
#include <lua.hpp>

namespace http_api
{

class Peer;
using PeerWeakPtr = std::weak_ptr<Peer>;

/** \brief Represents a handle to a request and information to form and send a response.
 *
 * This class stores the request and holds a weak handle to the Peer object that originated
 * the request. If the connection expires while this object exists \ref send() will fail
 * with a warning. Calling \ref send() on a \ref Responder twice or calling it on an
 * Responder that was not created by a Peer is an error and will most likely print a warning.
 *
 * \remark Thread-unsafe - objects of this class should only be accessed by one thread at a time.
 */
class Responder : NonCopyable
{
	/// Weak handle to the associated \ref Peer
	const PeerWeakPtr peerWeak;
	/// Unique ID that identifies which request this is. Used to identify calls to \refsend() on invalid Responders
	const RequestID requestID;
public:
	/// Request data
	const Request request;
	/// Response data. Can be freely mutated until \ref send() is called
	Response response;

	Responder(PeerWeakPtr peer, Request request, RequestID requestID);

	static int luaDelete(lua_State* L);
	static int luaSend(lua_State* L);

	static int luaGetRequestBody(lua_State* L);
	static int luaSetResponseBody(lua_State* L);

	static int luaGetRequestField(lua_State* L);
	static int luaSetResponseField(lua_State* L);

	static int luaGetRequestURL(lua_State* L);
	static int luaGetRequestMethod(lua_State* L);
	static int luaSetResponseStatus(lua_State* L);

	/** \brief Sends the data contained in \ref response
	 *
	 * Attempts to send a response based on data stored in \ref response. If the connection
	 * expired or this responder is invalid the operation will fail silently.
	 */
	void send();
};

} //namespace http_api

#endif // FS_HTTP_API_RESPONDER_H_8F52C86127F133F8E606D96A33069264
