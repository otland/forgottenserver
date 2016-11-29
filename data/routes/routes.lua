routes = {}
dofile("data/routes/scripts/api/players/online.lua")

local function dispatchRequest(responder)
    responder:setResponseField('Server', 'TFS/1.3')
    local route = routes[responder:getRequestURL()]
    if not route then
        responder:setResponseStatus(404, 'Not Found')
        return false
    end

    if not route.methods[responder:getRequestMethod()]  then
        responder:setResponseStatus(405, 'Method Not Allowed')
        return false
    end

    return route.handler(responder)
end

function handleRequest(responder)
    if not dispatchRequest(responder) then
        responder:send()
    end
    return true
end