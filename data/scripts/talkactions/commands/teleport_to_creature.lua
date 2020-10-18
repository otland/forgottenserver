local talk = TalkAction("/goto")

function talk.onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	local target = Creature(param)
	if target then
		player:teleportTo(target:getPosition())
	else
		player:sendCancelMessage("Creature not found.")
	end
	return false
end

talk:separator(" ")
talk:register()
