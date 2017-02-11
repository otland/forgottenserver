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

#ifndef FS_HTTP_API_ROUTER_H_BF19E77EB9613BA48A6343C06DD0B197
#define FS_HTTP_API_ROUTER_H_BF19E77EB9613BA48A6343C06DD0B197

#include "responder.h"
#include "baseevents.h"

namespace http_api
{

/** \brief Represents the lua HTTP API subsystem
 * This class is responsible for loading and maintaining the
 * lua HTTP API subsystem. When an API event occurs it dispatches
 * the apropriate lua callback.
 * \remark Thread-unsafe - objects of this class should only be manipulated
 * from within the dispatcher thread's context.
 */
class Router final
{
	/// Represents the lua interface handle used by the HTTP API subsystem
	LuaScriptInterface interface{"HTTP API Router interface"};
	/// Represents the handle to the request handling lua callback
	int32_t luaRequestHandlerId{-1};
	/// Represents the handle to the session creation lua callback
	int32_t luaSessionOpenId{-1};
	/// Represents the handle to the session destruction lua callback
	int32_t luaSessionCloseId{-1};

	void sendInternalServerError(Responder& responder) const;
public:
	Router();

	/** \brief Loads the lua HTTP callbacks
	 *
	 * Attempts to load the lua routing callbacks. In the event of failure
	 * it returns false and the API is disabled(HTTP 500 Internal Server Error is sent).
	 */
	bool loadRoutingFunctions();

	/** \brief Handles an incoming HTTP request
	 *
	 * Attempts to call the lua HTTP API subsystem's callback function to handle the
	 * incoming request. If the callback has not been loaded properly or cannot
	 * be called then a HTTP 500 Internal Server Error is sent as a response.
	 * \param[in] responder - the object that contains the request data and a handle
	 * to send a response
	 * \param[in] peerID - unique ID of a remote \ref Peer, used by lua to identify which
	 * connection this request belongs to
	 */
	void handleRequest(Responder responder, PeerID peerID);

	/** \brief Handles the opening of an HTTP connection and session
	 *
	 * Attempts to dispatch the lua session open handler which allows the API side
	 * to track open sessions. In the event of failure(e.g. when the handler is not present)
	 * no action is taken.
	 * \param[in] peerID - unique ID of a remote \ref Peer, used by lua to identify which
	 * connection this event belongs to
	 */
	void handleSessionOpen(PeerID peerID);

	/** \brief Handles the closing of an HTTP connection and session
	 *
	 * Attempts to dispatch the lua session close handler which allows the API side
	 * to track session close events and release any resources that are no longer
	 * needed.
	 * \param[in] peerID - unique ID of a remote \ref Peer, used by lua to identify which
	 * connection this event belongs to
	 */
	void handleSessionClose(PeerID peerID);
};

} //namespace http_api

#endif //FS_HTTP_API_ROUTER_H_BF19E77EB9613BA48A6343C06DD0B197