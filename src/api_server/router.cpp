#include "router.h"

namespace http
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
	std::cout << std::endl;
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
	std::cout << std::endl;
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
	std::cout << std::endl;
}

void Router::sendInternalServerError(http::Responder& responder) const
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

} //namespace http
