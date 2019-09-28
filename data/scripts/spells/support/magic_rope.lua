local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	local position = creature:getPosition()
	position:sendMagicEffect(CONST_ME_POFF)

	local tile = Tile(position)
	if not table.contains(ropeSpots, tile:getGround():getId()) and not tile:getItemById(14435) then
		creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		return false
	end

	tile = Tile(position:moveUpstairs())
	if not tile then
		creature:sendCancelMessage(RETURNVALUE_NOTENOUGHROOM)
		return false
	end

	creature:teleportTo(position, false)
	position:sendMagicEffect(CONST_ME_TELEPORT)
	return true
end

spell:name("Magic Rope")
spell:words("exani tera")
spell:group("support")
spell:vocation("druid", "elder druid", "knight", "elite knight", "paladin", "royal paladin", "sorcerer", "master sorcerer")
spell:id(76)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(9)
spell:mana(20)
spell:isSelfTarget(true)
spell:isAggressive(false)
spell:isPremium(true)
spell:register()
