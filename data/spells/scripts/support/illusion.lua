local condition = Condition(CONDITION_OUTFIT)
condition:setTicks(180000)

function onCastSpell(creature, variant)
	if not creature:isPlayer() then
		return false
	end

	local returnValue = RETURNVALUE_NOERROR

	local monsterType = MonsterType(variant:getString())
	if not monsterType then
		returnValue = RETURNVALUE_CREATUREDOESNOTEXIST
	end

	if returnValue == RETURNVALUE_NOERROR then
		if not getPlayerFlagValue(creature, PlayerFlag_CanIllusionAll) or not monsterType:isIllusionable() then
			returnValue = RETURNVALUE_NOTPOSSIBLE
		end
	end

	if returnValue ~= RETURNVALUE_NOERROR then
		creature:sendCancelMessage(returnValue)
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end

	condition:setOutfit(monsterType:getOutfit())
	creature:addCondition(condition)

	creature:getPosition():sendMagicEffect(CONST_ME_MAGIC_RED)
	return true
end
