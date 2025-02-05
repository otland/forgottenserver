local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	local house = creature:getTile():getHouse()
	if not house or not house:canEditAccessList(SUBOWNER_LIST, creature) then
		creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end

	creature:setEditHouse(house, SUBOWNER_LIST)
	creature:sendHouseWindow(house, SUBOWNER_LIST)
	return true
end

spell:name("Invite Subowners")
spell:words("aleta som")
spell:isAggressive(false)
spell:register()
