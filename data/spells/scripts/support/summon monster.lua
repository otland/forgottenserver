function onCastSpell(creature, var)
	local param = variantToString(var)
	local mType = MonsterType(param)

	if not mType then
		creature:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_NOTPOSSIBLE))
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end

	if not getPlayerFlagValue(creature:getId(), PlayerFlag_CanSummonAll) then
		if not mType:isSummonable() then
			creature:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_NOTPOSSIBLE))
			creature:getPosition():sendMagicEffect(CONST_ME_POFF)
			return false
		end

		if creature:getMana() < mType:getManaCost() then
			creature:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_NOTENOUGHMANA))
			creature:getPosition():sendMagicEffect(CONST_ME_POFF)
			return false
		end

		if #creature:getSummons() >= 2 then
			creature:sendTextMessage(MESSAGE_STATUS_SMALL, 'You cannot summon more creatures.')
			creature:getPosition():sendMagicEffect(CONST_ME_POFF)
			return false
		end
	end

	local playerPos = creature:getPosition()
	local summon = Game.createMonster(param, playerPos, true)
	if not summon then
		creature:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_NOTENOUGHROOM))
		playerPos:sendMagicEffect(CONST_ME_POFF)
		return false
	end

	summon:setMaster(creature:getId())
	playerPos:sendMagicEffect(CONST_ME_MAGIC_BLUE)
	summon:getPosition():sendMagicEffect(CONST_ME_TELEPORT)
	return true
end