function onCastSpell(creature, variant)
	local monsterName = variant:getString()
	local monsterType = MonsterType(monsterName)

	if not monsterType then
		creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end

	if not getPlayerFlagValue(creature, PlayerFlag_CanSummonAll) then
		if not monsterType:isSummonable() then
			creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
			creature:getPosition():sendMagicEffect(CONST_ME_POFF)
			return false
		end

		if #creature:getSummons() >= 2 then
			creature:sendCancelMessage("You cannot summon more creatures.")
			creature:getPosition():sendMagicEffect(CONST_ME_POFF)
			return false
		end
	end

	local manaCost = monsterType:getManaCost()
	if creature:getMana() < manaCost and not getPlayerFlagValue(creature, PlayerFlag_HasInfiniteMana) then
		creature:sendCancelMessage(RETURNVALUE_NOTENOUGHMANA)
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end

	local position = creature:getPosition()
	local summon = Game.createMonster(monsterName, position, true)
	if not summon then
		creature:sendCancelMessage(RETURNVALUE_NOTENOUGHROOM)
		position:sendMagicEffect(CONST_ME_POFF)
		return false
	end

	creature:addMana(-manaCost)
	creature:addManaSpent(manaCost)
	creature:addSummon(summon)
	position:sendMagicEffect(CONST_ME_MAGIC_BLUE)
	summon:getPosition():sendMagicEffect(CONST_ME_TELEPORT)
	return true
end
