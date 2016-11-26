function onRequest(responder)
    local buf = {"{\"players\":["}
    for _, player in ipairs(Game.getPlayers()) do
        buf[#buf + 1] = string.format("{\"name\":%s}", player:getName())
        buf[#buf + 1] = ","
    end
    if #buf > 1 then
        buf[#buf] = "]}" --Replace trailing comma with array end
    else
        buf[#buf + 1] = "]}"
    end
    body = table.concat(buf, "")
    responder:setResponseStatus(200, "OK")
    responder:setResponseBody(body)
    responder:send()
    return true
end
