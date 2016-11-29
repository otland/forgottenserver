#include "../otpch.h"

#include "responder.h"
#include "peer.h"
#include "../luascript.h"

namespace http
{

Responder::Responder(PeerSharedPtr peer, Request request, RequestID requestID):
	peer(std::move(peer)),
	requestID(requestID),
	request(std::move(request))
{
	response.version = 11;
	//
}

void Responder::send()
{
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

} //namespace http
