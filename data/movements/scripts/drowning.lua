local condition = Condition(CONDITION_DROWN)
condition:setParameter(CONDITION_PARAM_DELAYED, 1)
condition:addDamage(5, 4000, -20)

function onStepIn(creature, item, position, fromPosition)
	if creature:isPlayer() then
		creature:addCondition(condition)
		creature:addAchievementProgress("Deep Sea Diver", 1000000)
	end
	return true
end

function onStepOut(creature, item, position, fromPosition)
	if creature:isPlayer() then
		creature:removeCondition(CONDITION_DROWN)
	end
	return true
end
