local Session = {permissions = {}, request = {}}
Router = {sessions = {}, routes = {}}

function Session:handleRequest(responder)
	self.responder = responder
	self.request.url = responder:getRequestURL()
	self.request.method = responder:getRequestMethod()
	local route = Router:find(self.request.url)
	if not route then
		responder:setResponseStatus(404, 'Not Found')
		return
	end

	local method = route.methods and route.methods[self.request.method]
	if route.methods and method == nil then
		responder:setResponseStatus(405, 'Method Not Allowed')
		return
	end
	method = method or {}

	if not Router.authenticator:hasPermissions(self, method) then
		responder:setResponseStatus(403, 'Forbidden')
		return
	end

	responder:setResponseStatus(200, 'OK')
	return route.handler(self, session)
end

function Session:new(s, peerID)
	session = s or {}
	setmetatable(session, self)
	self.__index = self
	session.peerID = peerID
	return session
end

function Router:handleRequest(responder, peerID)
	responder:setResponseField('Server', 'TFS/1.3')
	local session = self.sessions[peerID]
	if not session:handleRequest(responder) then
		responder:send()
	end
	return true
end

function Router:handleSessionOpen(peerID)
	self.sessions[peerID] = Session:new({router = self}, peerID)
	return true
end

function Router:handleSessionClose(peerID)
	local session = self.sessions[peerID]
	self.sessions[peerID] = nil
	self.authenticator.onSessionClose(session)
	return true
end

function Router:register(url, handler, methods)
	local route = {handler = handler, methods = methods}
	self.routes[url] = route
	return self
end

function Router:find(url)
	for key, route in pairs(self.routes) do
		if string.match(url, key) then
			return route
		end
	end
end
