function onCastSpell(creature, variant, isHotkey)
	local target = Tile(variant:getPosition()):getTopVisibleThing(creature)
	if not target or not target:isMonster() then
		creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end

	local monsterType = target:getType()
	if not getPlayerFlagValue(creature, PlayerFlag_CanConvinceAll) then
		if not monsterType:isConvinceable() or #creature:getSummons() >= 2 then
			creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
			creature:getPosition():sendMagicEffect(CONST_ME_POFF)
			return false
		end
	end

	local manaCost = target:getType():getManaCost()
	if creature:getMana() < manaCost and not getPlayerFlagValue(creature, PlayerFlag_HasInfiniteMana) then
		creature:sendCancelMessage(RETURNVALUE_NOTENOUGHMANA)
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end

	creature:addMana(-manaCost)
	creature:addManaSpent(manaCost)

	target:setMaster(creature)
	creature:getPosition():sendMagicEffect(CONST_ME_MAGIC_RED)
	return true
end
