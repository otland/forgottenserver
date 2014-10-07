function onCastSpell(creature, var)
	local param = variantToString(var)
	local mType = MonsterType(param)
	local playerPos = creature:getPosition()

	if not mType then
		creature:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_NOTPOSSIBLE))
		playerPos:sendMagicEffect(CONST_ME_POFF)
		return false
	end

	if not getPlayerFlagValue(creature:getId(), PlayerFlag_CanSummonAll) then
		if not mType:isSummonable() then
			creature:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_NOTPOSSIBLE))
			playerPos:sendMagicEffect(CONST_ME_POFF)
			return false
		end

		if creature:getMana() < mType:getManaCost() then
			creature:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_NOTENOUGHMANA))
			playerPos:sendMagicEffect(CONST_ME_POFF)
			return false
		end

		if #creature:getSummons() >= 2 then
			creature:sendTextMessage(MESSAGE_STATUS_SMALL, 'You cannot summon more creatures.')
			playerPos:sendMagicEffect(CONST_ME_POFF)
			return false
		end
	end

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
