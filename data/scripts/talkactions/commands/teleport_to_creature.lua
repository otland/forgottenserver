local talkaction = TalkAction("/goto")

function talkaction.onSay(player, words, param)
	local target = Creature(param)
	if target then
		player:teleportTo(target:getPosition())
	else
		player:sendCancelMessage("Creature not found.")
	end
	return false
end

talkaction:separator(" ")
talkaction:access(true)
talkaction:register()
