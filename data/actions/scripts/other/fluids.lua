local drunk = Condition(CONDITION_DRUNK)
drunk:setParameter(CONDITION_PARAM_TICKS, 60000)

local poison = Condition(CONDITION_POISON)
poison:setParameter(CONDITION_PARAM_DELAYED, true)
poison:setParameter(CONDITION_PARAM_MINVALUE, -50)
poison:setParameter(CONDITION_PARAM_MAXVALUE, -120)
poison:setParameter(CONDITION_PARAM_STARTVALUE, -5)
poison:setParameter(CONDITION_PARAM_TICKINTERVAL, 4000)
poison:setParameter(CONDITION_PARAM_FORCEUPDATE, true)

local fluidMessage = {
	[3] = "Aah...",
	[4] = "Urgh!",
	[5] = "Mmmh.",
	[7] = "Aaaah...",
	[10] = "Aaaah...",
	[11] = "Urgh!",
	[13] = "Urgh!",
	[15] = "Aah...",
	[19] = "Urgh!",
	[27] = "Aah...",
	[43] = "Aaaah..."
}

local distillery = {[5513] = 5469, [5514] = 5470}

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local targetItemType = ItemType(target.itemid)
	if targetItemType and targetItemType:isFluidContainer() then
		if target.type == 0 and item.type ~= 0 then
			target:transform(target:getId(), item.type)
			item:transform(item:getId(), 0)
			return true
		elseif target.type ~= 0 and item.type == 0 then
			item:transform(item:getId(), target.type)
			target:transform(target:getId(), 0)
			return true
		end
	end

	if target.itemid == 1 then
		if item.type == 0 then
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "It is empty.")
		elseif target.uid == player.uid then
			if table.contains({3, 15, 43}, item.type) then
				player:addCondition(drunk)
			elseif item.type == 4 then
				player:addCondition(poison)
			elseif item.type == 7 then
				player:addMana(math.random(50, 150))
				fromPosition:sendMagicEffect(CONST_ME_MAGIC_BLUE)
			elseif item.type == 10 then
				player:addHealth(60)
				fromPosition:sendMagicEffect(CONST_ME_MAGIC_BLUE)
			end
			player:say(fluidMessage[item.type] or "Gulp.", TALKTYPE_MONSTER_SAY)
			item:transform(item:getId(), 0)
		else
			Game.createItem(2016, item.type, toPosition):decay()
			item:transform(item:getId(), 0)
		end
	else
		local fluidSource = targetItemType and targetItemType:getFluidSource() or 0
		if fluidSource ~= 0 then
			item:transform(item:getId(), fluidSource)
		elseif table.contains(distillery, target.itemid) then
			local tmp = distillery[target.itemid]
			if tmp then
				item:transform(item:getId(), 0)
			else
				player:sendCancelMessage("You have to process the bunch into the distillery to get rum.")
			end
		elseif item.type == 0 then
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "It is empty.")
		else
			if toPosition.x == CONTAINER_POSITION then
				toPosition = player:getPosition()
			end
			Game.createItem(2016, item.type, toPosition):decay()
			item:transform(item:getId(), 0)
		end
	end
	return true
end
