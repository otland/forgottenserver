function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	local target = Creature(param)
	if target then
		player:teleportTo(target:getPosition(), true)
		player:getPosition():sendMagicEffect(CONST_ME_TELEPORT)
	else
		player:sendCancelMessage("Creature not found.")
		player:getPosition():sendMagicEffect(CONST_ME_POFF)
	end
	return false
end
