local cursed = Condition(CONDITION_CURSED)
cursed:setParameter(CONDITION_PARAM_DELAYED, true) -- condition will delay the first damage from when it's added
cursed:setParameter(CONDITION_PARAM_MINVALUE, -800) -- minimum damage the condition can do at total
cursed:setParameter(CONDITION_PARAM_MAXVALUE, -1200) -- maximum damage
cursed:setParameter(CONDITION_PARAM_STARTVALUE, -1) -- the damage the condition will do on the first hit
cursed:setParameter(CONDITION_PARAM_TICKINTERVAL, 4000) -- delay between damages
cursed:setParameter(CONDITION_PARAM_FORCEUPDATE, true) -- re-update condition when adding it(ie. min/max value)

local clawOfTheNoxiousSpawn = Action()

function clawOfTheNoxiousSpawn.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if item == player:getSlotItem(CONST_SLOT_RING) then
		if math.random(100) <= 5 then
			player:addCondition(cursed)
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You are cursed by The Noxious Spawn!")
			item:transform(10312)
			item:decay()
			player:getPosition():sendMagicEffect(CONST_ME_MAGIC_RED)
		else
			player:removeCondition(CONDITION_POISON)
			item:transform(10311)
			item:decay()
			player:getPosition():sendMagicEffect(CONST_ME_MAGIC_GREEN)
		end
		return true
	end
	return false
end

clawOfTheNoxiousSpawn:id(10309)
clawOfTheNoxiousSpawn:register()
