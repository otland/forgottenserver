local broadcast = TalkAction("/b")

function broadcast.onSay(player, words, param)
	if not player:hasFlag(PlayerFlag_CanBroadcast) then
		return true
	end

	if(param == "") then
		player:sendCancelMessage("Command param required.")
		return false
	end

	print("> " .. player:getName() .. " broadcasted: \"" .. param .. "\".")
	for _, targetPlayer in ipairs(Game.getPlayers()) do
		targetPlayer:sendPrivateMessage(player, param, TALKTYPE_BROADCAST)
	end
	return false
end

broadcast:separator(" ")
broadcast:register()
