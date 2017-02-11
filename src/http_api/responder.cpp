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

#include "otpch.h"

#include "responder.h"
#include "peer.h"
#include "luascript.h"

namespace http_api
{

Responder::Responder(PeerWeakPtr peer, Request request, RequestID requestID):
	peerWeak(std::move(peer)),
	requestID(requestID),
	request(std::move(request))
{
	response.version = 11;
	//
}

void Responder::send()
{
	auto peer = peerWeak.lock();
	if (peer == nullptr) {
		std::cerr << "HTTP API send error: Peer no longer connected" << std::endl;
		return;
	}
	peer->send(std::move(response), requestID);
}

int Responder::luaDelete(lua_State* L)
{
	Responder** responderPtr = LuaScriptInterface::getRawUserdata<Responder>(L, 1);
	if (responderPtr != nullptr && *responderPtr != nullptr) {
		delete *responderPtr;
		*responderPtr = nullptr;
	}
	return 0;
}

int Responder::luaSend(lua_State* L)
{
	//responder::send()
	Responder* responder = LuaScriptInterface::getUserdata<Responder>(L, 1);
	if (responder == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	responder->send();
	LuaScriptInterface::pushBoolean(L, true);
	return 1;
}

int Responder::luaGetRequestBody(lua_State* L)
{
	//responder::getRequestBody()
	Responder* responder = LuaScriptInterface::getUserdata<Responder>(L, 1);
	if (responder == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	LuaScriptInterface::pushString(L, responder->request.body);
	return 1;
}

int Responder::luaSetResponseBody(lua_State* L)
{
	//responder::setResponseBody(body)
	Responder* responder = LuaScriptInterface::getUserdata<Responder>(L, 1);
	if (responder == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	responder->response.body = LuaScriptInterface::getString(L, 2);
	LuaScriptInterface::pushBoolean(L, true);
	return 1;
}

int Responder::luaGetRequestField(lua_State* L)
{
	//responder:getRequestField(fieldName)
	const Responder* responder = LuaScriptInterface::getUserdata<Responder>(L, 1);
	if (responder == nullptr) {
		lua_pushnil(L);
		return 1;
	}
	const auto fieldName = LuaScriptInterface::getString(L, 2);
	const auto it = responder->request.fields.find(fieldName);
	if (it == responder->request.fields.end()) {
		lua_pushnil(L);
		return 1;
	}

	const auto& fieldValue = it->second;
	LuaScriptInterface::pushString(L, fieldValue);
	return 1;
}

int Responder::luaSetResponseField(lua_State* L)
{
	//responder:setResponseField(fieldName, fieldValue)
	Responder* responder = LuaScriptInterface::getUserdata<Responder>(L, 1);
	if (responder == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	auto fieldName = LuaScriptInterface::getString(L, 2);
	auto fieldValue = LuaScriptInterface::getString(L, 3);
	responder->response.fields.replace(fieldName, fieldValue);
	LuaScriptInterface::pushBoolean(L, true);
	return 1;
}

int Responder::luaGetRequestMethod(lua_State* L)
{
	//responder:getRequestMethod()
	Responder* responder = LuaScriptInterface::getUserdata<Responder>(L, 1);
	if (responder == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	LuaScriptInterface::pushString(L, responder->request.method);
	return 1;
}

int Responder::luaSetResponseStatus(lua_State* L)
{
	//responder:setResponseStatus(status, reason)
	Responder* responder = LuaScriptInterface::getUserdata<Responder>(L, 1);
	if (responder == nullptr) {
		lua_pushnil(L);
		return 1;
	}
	responder->response.status = LuaScriptInterface::getNumber<int>(L, 2);
	responder->response.reason = LuaScriptInterface::getString(L, 3);
	return 1;
}

int Responder::luaGetRequestURL(lua_State* L)
{
	//responder:getRequestURL()
	Responder* responder = LuaScriptInterface::getUserdata<Responder>(L, 1);
	if (responder == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	LuaScriptInterface::pushString(L, responder->request.url);
	return 1;
}

} //namespace http_api
