local drunk = Condition(CONDITION_DRUNK)
drunk:setParameter(CONDITION_PARAM_TICKS, 60000)

local poison = Condition(CONDITION_POISON)
poison:setParameter(CONDITION_PARAM_DELAYED, true)
poison:setParameter(CONDITION_PARAM_MINVALUE, -50)
poison:setParameter(CONDITION_PARAM_MAXVALUE, -120)
poison:setParameter(CONDITION_PARAM_STARTVALUE, -5)
poison:setParameter(CONDITION_PARAM_TICKINTERVAL, 4000)
poison:setParameter(CONDITION_PARAM_FORCEUPDATE, true)

local fluidType = {3, 4, 5, 7, 10, 11, 13, 15, 19}
local fluidMessage = {"Aah...", "Urgh!", "Mmmh.", "Aaaah...", "Aaaah...", "Urgh!", "Urgh!", "Aah...", "Urgh!"}

function onUse(player, item, fromPosition, targetEx, toPosition, isHotkey)
	local targetExType = targetEx:getType()
	if targetExType and targetExType:isFluidContainer() then
		if targetEx:getSubType() == 0 and item:getSubType() ~= 0 then
			targetEx:transform(targetEx:getId(), item:getSubType())
			item:transform(item:getId(), 0)
			return true
		elseif targetEx:getSubType() ~= 0 and item:getSubType() == 0 then
			targetEx:transform(targetEx:getId(), 0)
			item:transform(item:getId(), targetEx:getSubType())
			return true
		end
	end

	if targetEx:getId() == 1 then
		if item:getSubType() == 0 then
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "It is empty.")
		elseif targetEx:getUniqueId() == player:getId() then
			item:transform(item:getId(), 0)
			if item:getSubType() == 3 or item:getSubType() == 15 then
				player:addCondition(drunk)
			elseif item:getSubType() == 4 then
				player:addCondition(poison)
			elseif item:getSubType() == 7 then
				player:addMana(math.random(50, 150))
				fromPosition:sendMagicEffect(CONST_ME_MAGIC_BLUE)
			elseif item:getSubType() == 10 then
				player:addHealth(60)
				fromPosition:sendMagicEffect(CONST_ME_MAGIC_BLUE)
			end
			for i = 0, #fluidType do
				if item:getSubType() == fluidType[i] then
					player:say(fluidMessage[i], TALKTYPE_MONSTER_SAY)
					return true
				end
			end
			player:say("Gulp.", TALKTYPE_MONSTER_SAY)
		else
			item:transform(item:getId(), 0)
			Game.createItem(2016, item:getSubType(), toPosition):decay()
		end
	else
		local fluidSource = targetExType and targetExType:getFluidSource() or 0
		if fluidSource ~= 0 then
			item:transform(item:getId(), fluidSource)
		elseif item:getSubType() == 0 then
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "It is empty.")
		else
			if toPosition.x == CONTAINER_POSITION then
				toPosition = player:getPosition()
			end
			item:transform(item:getId(), 0)
			Game.createItem(2016, item:getSubType(), toPosition):decay()
		end
	end

	return true
end
