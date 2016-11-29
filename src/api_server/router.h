#ifndef FS_API_SERVER_ROUTER_H
#define FS_API_SERVER_ROUTER_H

#include "responder.h"
#include "../baseevents.h"
#include <unordered_map>
#include <functional>
namespace http
{

class Router final
{
	LuaScriptInterface interface{"HTTP API Router interface"};
	int32_t luaHandlerId{-1};
	void sendInternalServerError(Responder& responder) const;

public:
	Router();
	bool loadRoutingFunction();
	void handleRequest(Responder responder);
};

} //namespace http

#endif //FS_API_SERVER_ROUTER_H