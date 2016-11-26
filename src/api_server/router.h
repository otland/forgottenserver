#ifndef FS_API_SERVER_ROUTER_H
#define FS_API_SERVER_ROUTER_H

#include "responder.h"
#include "../baseevents.h"
#include <unordered_map>
#include <functional>
namespace http
{

class Route final : public Event
{
	std::string getScriptEventName() const final;
	bool configureEvent (const pugi::xml_node& node) final;
public:
	std::string uri;
	explicit Route (LuaScriptInterface*);
	using Pointer = std::unique_ptr<Route>;
	void handleRequest(Responder responder) const;
};

class Router final : public BaseEvents
{
	using Routes = std::unordered_map<std::string, Route::Pointer>;

	Routes routes;
	LuaScriptInterface interface{"HTTP API Router interface"};

	std::string getScriptBaseName() const final;
	LuaScriptInterface& getScriptInterface() final;
	Event* getEvent(const std::string& nodeName) final;
	bool registerEvent(Event* event, const pugi::xml_node& node) final;
	void clear() final;
public:
	Router();
	void handleRequest(Responder responder);
};

} //namespace http

#endif //FS_API_SERVER_ROUTER_H