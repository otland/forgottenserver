// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "responder.h"
#include "peer.h"
#include "../luascript.h"

namespace Http
{

Responder::Responder(PeerWeakPtr peer, Request request, RequestID requestID):
	peerWeak(std::move(peer)),
	requestID(requestID),
	request(std::move(request))
{
	response.version(11);
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
	auto* responder = LuaScriptInterface::getUserdata<Responder>(L, 1);
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
	auto* responder = LuaScriptInterface::getUserdata<Responder>(L, 1);
	if (responder == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	LuaScriptInterface::pushString(L, responder->request.body());
	return 1;
}

int Responder::luaSetResponseBody(lua_State* L)
{
	//responder::setResponseBody(body)
	auto* responder = LuaScriptInterface::getUserdata<Responder>(L, 1);
	if (responder == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	responder->response.body() = LuaScriptInterface::getString(L, 2);
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
	const auto it = responder->request.find(fieldName);
	if (it == responder->request.end()) {
		lua_pushnil(L);
		return 1;
	}

	const auto& fieldValue = it->value().to_string();
	LuaScriptInterface::pushString(L, fieldValue);
	return 1;
}

int Responder::luaSetResponseField(lua_State* L)
{
	//responder:setResponseField(fieldName, fieldValue)
	auto* responder = LuaScriptInterface::getUserdata<Responder>(L, 1);
	if (responder == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	auto fieldName = LuaScriptInterface::getString(L, 2);
	auto fieldValue = LuaScriptInterface::getString(L, 3);
	responder->response.set(fieldName, fieldValue);
	LuaScriptInterface::pushBoolean(L, true);
	return 1;
}

int Responder::luaGetRequestMethod(lua_State* L)
{
	//responder:getRequestMethod()
	auto* responder = LuaScriptInterface::getUserdata<Responder>(L, 1);
	if (responder == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	LuaScriptInterface::pushString(L, std::string(responder->request.method_string()));
	return 1;
}

int Responder::luaSetResponseStatus(lua_State* L)
{
	//responder:setResponseStatus(status, reason)
	auto* responder = LuaScriptInterface::getUserdata<Responder>(L, 1);
	if (responder == nullptr) {
		lua_pushnil(L);
		return 1;
	}
	responder->response.result(LuaScriptInterface::getNumber<int>(L, 2));
	responder->response.body() = LuaScriptInterface::getString(L, 3);
	return 1;
}

int Responder::luaGetRequestURL(lua_State* L)
{
	//responder:getRequestURL()
	auto* responder = LuaScriptInterface::getUserdata<Responder>(L, 1);
	if (responder == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	LuaScriptInterface::pushString(L, std::string(responder->request.target()));
	return 1;
}

} //namespace Http
