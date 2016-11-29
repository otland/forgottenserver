#include "router.h"

namespace http
{

Router::Router()
{
	interface.initState();
}

void Router::handleRequest(http::Responder responder)
{
	if (luaHandlerId == -1) {
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
	env->setScriptId(luaHandlerId, &interface);

	lua_State* L = interface.getLuaState();

	interface.pushFunction(luaHandlerId);
	Responder* responderPtr;
	LuaScriptInterface::pushUserdata<Responder>(L, responderPtr = new Responder(std::move(responder)));
	LuaScriptInterface::setMetatable(L, -1, "Responder");

	if (interface.callFunction(1) == false) {
		std::cerr << "HTTP API handleRequest lua function error: " << interface.getLastLuaError()  << std::endl;
		sendInternalServerError(*responderPtr);
	}
}

void Router::sendInternalServerError(http::Responder& responder) const
{
	responder.response.status = 500;
	responder.response.reason = "Internal Server Error";
	responder.send();
}

bool Router::loadRoutingFunction()
{
	if (interface.loadFile("data/routes/routes.lua") == -1) {
		std::cerr << "HTTP API loadRoutingFunction loadFile failed: " << interface.getLastLuaError() << std::endl;
		return false;
	}
	luaHandlerId = interface.getEvent("handleRequest");
	if (luaHandlerId == -1) {
		std::cerr << "HTTP API loadRoutingFunction getEvent failed: " << interface.getLastLuaError() << std::endl;
		return false;
	}
	return true;
}

} //namespace http
