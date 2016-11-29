local function GetApiPlayersOnline(responder)
    local buf = {'{\"players\":['}
    for _, player in ipairs(Game.getPlayers()) do
        buf[#buf + 1] = string.format('{"name":"%s"}', player:getName())
        buf[#buf + 1] = ','
    end
    if #buf > 1 then
        buf[#buf] = ']}' --Replace trailing comma with array end
    else
        buf[#buf + 1] = ']}'
    end
    local body = table.concat(buf, '')
    responder:setResponseStatus(200, "OK")
    responder:setResponseBody(body)
end

routes['/api/players/online'] = {handler = GetApiPlayersOnline, methods = {GET = true}}