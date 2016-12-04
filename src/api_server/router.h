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
	int32_t luaRequestHandlerId{-1};
	int32_t luaSessionOpenId{-1};
	int32_t luaSessionCloseId{-1};
	void sendInternalServerError(Responder& responder) const;

public:
	Router();
	bool loadRoutingFunctions();
	void handleRequest(Responder responder, PeerID peerID);
	void handleSessionOpen(PeerID peerID);
	void handleSessionClose(PeerID peerID);
};

} //namespace http

#endif //FS_API_SERVER_ROUTER_H