local talkaction = TalkAction("!pos")

function talkaction.onSay(player, words, param)
    local position = player:getPosition()
    player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Your current position is: " .. position.x .. ", " .. position.y .. ", " .. position.z .. ".")
    return false
end

talkaction:separator(" ")
talkaction:register()
