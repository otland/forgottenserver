local condition = Condition(CONDITION_OUTFIT)
condition:setTicks(180000)

function onCastSpell(creature, variant)
	local returnValue = RETURNVALUE_NOERROR
	local monsterType = MonsterType(variant:getString())
	if not monsterType then
		returnValue = RETURNVALUE_CREATUREDOESNOTEXIST
	elseif not getPlayerFlagValue(creature, PlayerFlag_CanIllusionAll) and not monsterType:isIllusionable() then
		returnValue = RETURNVALUE_NOTPOSSIBLE
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
