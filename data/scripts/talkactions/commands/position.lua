local talkaction = TalkAction("/pos")

function talkaction.onSay(player, words, param)
    if param == "" then
        player:sendCancelMessage("Command requires parameters. Usage: /pos x,y,z")
        return false
    end

    local split = param:split(",")
    if #split ~= 3 then
        player:sendCancelMessage("Invalid position format. Use: /pos x,y,z")
        return false
    end

    local x = tonumber(split[1])
    local y = tonumber(split[2])
    local z = tonumber(split[3])
    if not x or not y or not z then
        player:sendCancelMessage("Invalid position values.")
        return false
    end

    local position = Position(x, y, z)
    player:teleportTo(position)
    player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have been teleported to position: " .. position.x .. ", " .. position.y .. ", " .. position.z)
    return false
end

talkaction:separator(" ")
talkaction:access(true)
talkaction:accountType(ACCOUNT_TYPE_GOD)
talkaction:register()
