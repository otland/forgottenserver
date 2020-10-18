local talk = TalkAction("/t")

function talk.onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	player:teleportTo(player:getTown():getTemplePosition())
	return false
end

talk:register()
