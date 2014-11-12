local condition = Condition(CONDITION_OUTFIT, CONDITIONID_COMBAT)
condition:setTicks(180000)

function onCastSpell(creature, var)
	local mType = MonsterType(variantToString(var))

	if not mType then
		creature:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_CREATUREDOESNOTEXIST))
		return false
	end

	if not getPlayerFlagValue(creature:getId(), PlayerFlag_CanIllusionAll) then
		if not mType:isIllusionable() then
			creature:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_NOTPOSSIBLE))
			creature:getPosition():sendMagicEffect(CONST_ME_POFF)
			return false
		end
	end

	condition:setOutfit(mType:getOutfit())
	creature:addCondition(condition)
	creature:getPosition():sendMagicEffect(CONST_ME_MAGIC_RED)
	return true
end