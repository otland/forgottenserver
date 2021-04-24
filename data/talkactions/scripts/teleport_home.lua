function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	player:teleportTo(player:getTown():getTemplePosition(), true)
	player:getPosition():sendMagicEffect(CONST_ME_TELEPORT)
	return false
end
