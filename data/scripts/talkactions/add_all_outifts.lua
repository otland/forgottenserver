local talk = TalkAction("/addalloutifts")

function talk.onSay(player, words, param)
	if not player:getGroup():getAccess() then
        return true
    end

    local targetPlayer = Player(param)
    if not targetPlayer then
        player:sendCancelMessage("Player not found.")
        return false
    end

    targetPlayer:addAddonToAllOutfits()
    player:sendTextMessage(MESSAGE_INFO_DESCR, "All outfits have been delivered to " .. targetPlayer:getName() .. ".")
    targetPlayer:sendTextMessage(MESSAGE_INFO_DESCR, "You have received all outfits from " .. player:getName() .. ".")
    return false
end

talk:separator(" ")
talk:register()
