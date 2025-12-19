local talkaction = TalkAction("/a")

function talkaction.onSay(player, words, param)
	local steps = tonumber(param)
	if not steps then
		return false
	end

	local position = player:getPosition()
	position:getNextPosition(player:getDirection(), steps)

	position = player:getClosestFreePosition(position, false)
	if position.x == 0 then
		player:sendCancelMessage("You cannot teleport there.")
		return false
	end

	player:teleportTo(position)
	return false
end

talkaction:separator(" ")
talkaction:access(true)
talkaction:register()
