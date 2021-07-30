local condition = Condition(CONDITION_DROWN)
condition:setParameter(CONDITION_PARAM_PERIODICDAMAGE, -20)
condition:setParameter(CONDITION_PARAM_TICKS, -1)
condition:setParameter(CONDITION_PARAM_TICKINTERVAL, 2000)

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
