// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "router.h"

namespace HttpApi
{

Router::Router()
{
	interface.initState();
}

void Router::handleRequest(Responder responder, PeerId peerId)
{
	if (luaRequestHandlerId == -1) {
		std::cerr << "HTTP API handleRequest lua function not found" << std::endl;
		sendInternalServerError(responder);
		return;
	}

	if (!LuaScriptInterface::reserveScriptEnv()) {
		std::cerr << "HTTP API handleRequest Call stack overflow" << std::endl;
		sendInternalServerError(responder);
		return;
	}

	ScriptEnvironment* env = LuaScriptInterface::getScriptEnv();
	env->setScriptId(luaRequestHandlerId, &interface);

	lua_State* L = interface.getLuaState();

	interface.pushFunction(luaRequestHandlerId);
	lua_getglobal(L, "Router");
	Responder* responderPtr;
	LuaScriptInterface::pushUserdata<Responder>(L, responderPtr = new Responder(std::move(responder)));
	LuaScriptInterface::setMetatable(L, -1, "Responder");

	lua_pushinteger(L, peerId);

	if (!interface.callFunction(3)) {
		std::cerr << "HTTP API handleRequest lua function error: " << interface.getLastLuaError()  << std::endl;
		sendInternalServerError(*responderPtr);
	}
}

void Router::handleSessionOpen(PeerId peerId)
{
	if (luaSessionOpenId == -1) {
		std::cerr << "HTTP API handleSessionOpen lua function not found" << std::endl;
		return;
	}

	if (!LuaScriptInterface::reserveScriptEnv()) {
		std::cerr << "HTTP API handleSessionOpen Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = LuaScriptInterface::getScriptEnv();
	env->setScriptId(luaSessionOpenId, &interface);

	lua_State* L = interface.getLuaState();

	interface.pushFunction(luaSessionOpenId);
	lua_getglobal(L, "Router");
	lua_pushinteger(L, peerId);

	if (!interface.callFunction(2)) {
		std::cerr << "HTTP API handleSessionOpen lua function error: " << interface.getLastLuaError()  << std::endl;
	}
}

void Router::handleSessionClose(PeerId peerId)
{
	if (luaSessionCloseId == -1) {
		std::cerr << "HTTP API handleSessionClose lua function not found" << std::endl;
		return;
	}

	if (!LuaScriptInterface::reserveScriptEnv()) {
		std::cerr << "HTTP API handleSessionClose Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = LuaScriptInterface::getScriptEnv();
	env->setScriptId(luaSessionCloseId, &interface);

	lua_State* L = interface.getLuaState();

	interface.pushFunction(luaSessionCloseId);
	lua_getglobal(L, "Router");
	lua_pushinteger(L, peerId);

	if (!interface.callFunction(2)) {
		std::cerr << "HTTP API handleSessionClose lua function error: " << interface.getLastLuaError()  << std::endl;
	}
}

void Router::sendInternalServerError(HttpApi::Responder& responder) const
{
	responder.response.result(boost::beast::http::status::internal_server_error);
	responder.response.body() = "Internal Server Error";
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

} //namespace HttpApi
