local condition = Condition(CONDITION_ATTRIBUTES)
condition:setParameter(CONDITION_PARAM_TICKS, 60 * 60 * 1000)
condition:setParameter(CONDITION_PARAM_SKILL_FISHING, 50)

local exhaust = Condition(CONDITION_EXHAUST_HEAL)
exhaust:setParameter(CONDITION_PARAM_TICKS, getConfigInfo('timeBetweenActions'))

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if SPECIAL_FOODS[item.itemid] == nil then
		return false
	end

	local sound = SPECIAL_FOODS[item.itemid][1]

	if not(player:addCondition(condition) or player:addCondition(exhaust)) then
		return false
	end

	item:remove(1)
	player:say(sound, TALKTYPE_MONSTER_SAY)
	return true
end