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

function onUse(cid, item, fromPosition, itemEx, toPosition)
	local itemExType = ItemType(itemEx.itemid)
	if itemExType and itemExType:isFluidContainer() then
		if itemEx.type == 0 and item.type ~= 0 then
			Item(itemEx.uid):transform(itemEx.itemid, item.type)
			Item(item.uid):transform(item.itemid, 0)
			return true
		elseif itemEx.type ~= 0 and item.type == 0 then
			Item(itemEx.uid):transform(itemEx.itemid, 0)
			Item(item.uid):transform(item.itemid, itemEx.type)
			return true
		end
	end

	if itemEx.itemid == 1 then
		if item.type == 0 then
			Player(cid):sendTextMessage(MESSAGE_STATUS_SMALL, "It is empty.")
		elseif itemEx.uid == cid then
			local player = Player(cid)
			Item(item.uid):transform(item.itemid, 0)
			if item.type == 3 or item.type == 15 then
				doTargetCombatCondition(0, cid, drunk, CONST_ME_NONE)
			elseif item.type == 4 then
				doTargetCombatCondition(0, cid, poison, CONST_ME_NONE)
			elseif item.type == 7 then
				player:addMana(math.random(50, 150))
				fromPosition:sendMagicEffect(CONST_ME_MAGIC_BLUE)
			elseif item.type == 10 then
				player:addHealth(60)
				fromPosition:sendMagicEffect(CONST_ME_MAGIC_BLUE)
			end
			for i = 0, #fluidType do
				if item.type == fluidType[i] then
					player:say(fluidMessage[i], TALKTYPE_MONSTER_SAY)
					return true
				end
			end
			player:say("Gulp.", TALKTYPE_MONSTER_SAY)
		else
			Item(item.uid):transform(item.itemid, 0)
			Game.createItem(2016, item.type, toPosition):decay()
		end
	else
		local fluidSource = itemExType and itemExType:getFluidSource() or 0
		if fluidSource ~= 0 then
			Item(item.uid):transform(item.itemid, fluidSource)
		elseif item.type == 0 then
			Player(cid):sendTextMessage(MESSAGE_STATUS_SMALL, "It is empty.")
		else
			if toPosition.x == CONTAINER_POSITION then
				toPosition = Player(cid):getPosition()
			end
			Item(item.uid):transform(item.itemid, 0)
			Game.createItem(2016, item.type, toPosition):decay()
		end
	end

	return true
end
