function onCastSpell(player, variant)
	local house = player:getTile():getHouse()
	if not house then
		return false
	end

	if house:canEditAccessList(SUBOWNER_LIST, player) then
		player:setEditHouse(house, SUBOWNER_LIST)
		player:sendHouseWindow(house, SUBOWNER_LIST)
	else
		player:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		player:getPosition():sendMagicEffect(CONST_ME_POFF)
	end
	return true
end
