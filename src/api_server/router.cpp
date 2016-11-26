#include "router.h"
#include <pugixml.hpp>
namespace http
{

Route::Route(LuaScriptInterface* interface):
	Event(interface)
{
	//
}

std::string Route::getScriptEventName() const
{
	return "onRequest";
}

bool Route::configureEvent(const pugi::xml_node& node)
{
	return true;
}

void Route::handleRequest(Responder responder) const
{
	if (!scriptInterface->reserveScriptEnv()) {
		std::cerr << "HTTP API handleRequest Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(scriptId, scriptInterface);

	lua_State* L = scriptInterface->getLuaState();

	scriptInterface->pushFunction(scriptId);

	LuaScriptInterface::pushUserdata<Responder>(L, new Responder(std::move(responder)));
	LuaScriptInterface::setMetatable(L, -1, "Responder");
	scriptInterface->callFunction(1);
}

Router::Router()
{
	interface.initState();
}

std::string Router::getScriptBaseName() const
{
	return "routes";
}

Event* Router::getEvent(const std::string& nodeName)
{
	if (strcasecmp(nodeName.c_str(), "route") != 0) {
		return nullptr;
	}
	return new Route{&interface};
}

LuaScriptInterface& Router::getScriptInterface()
{
	return interface;
}

bool Router::registerEvent(Event* event, const pugi::xml_node& node)
{
	auto& route = *static_cast<Route*>(event);
	if (auto attribute = node.attribute("url")) {
		route.uri = attribute.as_string();
		routes.emplace(route.uri, Route::Pointer(&route));
		return true;
	}
	return false;
}

void Router::handleRequest(Responder responder)
{
	auto it = routes.find(responder.request.url);
	if (it == routes.end()) {
		responder.response.status = 404;
		responder.response.reason = "Not found";
		responder.send();
		return;
	}
	it->second->handleRequest(std::move(responder));
}

void Router::clear()
{
	routes.clear();
}

} //namespace http
