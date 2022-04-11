// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_HTTP_RESPONDER_H
#define FS_HTTP_RESPONDER_H

#include "../tools.h"
#include "common.h"
#include <lua.hpp>

namespace Http
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

} //namespace Http

#endif // FS_HTTP_RESPONDER_H
