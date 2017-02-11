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

#include "router.h"

namespace http_api
{

Router::Router()
{
	interface.initState();
}

void Router::handleRequest(Responder responder, PeerID peerID)
{
	if (luaRequestHandlerId == -1) {
		std::cerr << "HTTP API handleRequest lua function not found" << std::endl;
		sendInternalServerError(responder);
		return;
	}

	if (!interface.reserveScriptEnv()) {
		std::cerr << "HTTP API handleRequest Call stack overflow" << std::endl;
		sendInternalServerError(responder);
		return;
	}

	ScriptEnvironment* env = interface.getScriptEnv();
	env->setScriptId(luaRequestHandlerId, &interface);

	lua_State* L = interface.getLuaState();

	interface.pushFunction(luaRequestHandlerId);
	lua_getglobal(L, "Router");
	Responder* responderPtr;
	LuaScriptInterface::pushUserdata<Responder>(L, responderPtr = new Responder(std::move(responder)));
	LuaScriptInterface::setMetatable(L, -1, "Responder");

	lua_pushinteger(L, peerID);

	if (interface.callFunction(3) == false) {
		std::cerr << "HTTP API handleRequest lua function error: " << interface.getLastLuaError()  << std::endl;
		sendInternalServerError(*responderPtr);
	}
}

void Router::handleSessionOpen(PeerID peerID)
{
	if (luaSessionOpenId == -1) {
		std::cerr << "HTTP API handleSessionOpen lua function not found" << std::endl;
		return;
	}

	if (!interface.reserveScriptEnv()) {
		std::cerr << "HTTP API handleSessionOpen Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = interface.getScriptEnv();
	env->setScriptId(luaSessionOpenId, &interface);

	lua_State* L = interface.getLuaState();

	interface.pushFunction(luaSessionOpenId);
	lua_getglobal(L, "Router");
	lua_pushinteger(L, peerID);

	if (interface.callFunction(2) == false) {
		std::cerr << "HTTP API handleSessionOpen lua function error: " << interface.getLastLuaError()  << std::endl;
	}
}

void Router::handleSessionClose(PeerID peerID)
{
	if (luaSessionCloseId == -1) {
		std::cerr << "HTTP API handleSessionClose lua function not found" << std::endl;
		return;
	}

	if (!interface.reserveScriptEnv()) {
		std::cerr << "HTTP API handleSessionClose Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = interface.getScriptEnv();
	env->setScriptId(luaSessionCloseId, &interface);

	lua_State* L = interface.getLuaState();

	interface.pushFunction(luaSessionCloseId);
	lua_getglobal(L, "Router");
	lua_pushinteger(L, peerID);

	if (interface.callFunction(2) == false) {
		std::cerr << "HTTP API handleSessionClose lua function error: " << interface.getLastLuaError()  << std::endl;
	}
}

void Router::sendInternalServerError(http_api::Responder& responder) const
{
	responder.response.status = 500;
	responder.response.reason = "Internal Server Error";
	responder.send();
}

bool Router::loadRoutingFunctions()
{
	if (interface.loadFile("data/routes/lib/routes.lua") == -1) {
		std::cerr << "HTTP API loadRoutingFunction loadFile failed: " << interface.getLastLuaError() << std::endl;
		return false;
	}

	if (interface.loadFile("data/routes/routes.lua") == -1) {
		std::cerr << "HTTP API loadRoutingFunction loadFile failed: " << interface.getLastLuaError() << std::endl;
		return false;
	}

	luaRequestHandlerId = interface.getMetaEvent("Router", "handleRequest");
	if (luaRequestHandlerId == -1) {
		std::cerr << "HTTP API loadRoutingFunction handleRequest failed: " << interface.getLastLuaError() << std::endl;
		return false;
	}

	luaSessionOpenId = interface.getMetaEvent("Router", "handleSessionOpen");
	if (luaSessionOpenId == -1) {
		std::cerr << "HTTP API loadRoutingFunction onSessionOpen failed: " << interface.getLastLuaError() << std::endl;
		return false;
	}

	luaSessionCloseId = interface.getMetaEvent("Router", "handleSessionClose");
	if (luaSessionCloseId == -1) {
		std::cerr << "HTTP API loadRoutingFunction onSessionClose failed: " << interface.getLastLuaError() << std::endl;
		return false;
	}

	return true;
}

} //namespace http_api
