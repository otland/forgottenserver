local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	local house = creature:getTile():getHouse()
	if not house or not house:canEditAccessList(GUEST_LIST, creature) then
		creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end

	creature:setEditHouse(house, GUEST_LIST)
	creature:sendHouseWindow(house, GUEST_LIST)
	return true
end

spell:name("House Guest List")
spell:words("aleta sio")
spell:isAggressive(false)
spell:register()
