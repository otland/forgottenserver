function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local sayParam, textParam, removeItem = "", "", true
	if item.itemid == 9992 then -- Rotworm Stew
		player:addHealth(math.max(1, player:getMaxHealth() - player:getHealth()))
		textParam = "Your health was refilled completely."
		sayParam = "Gulp."
	elseif item.itemid == 9993 then -- Hydra Tongue Salad
		local condition = {CONDITION_POISON, CONDITION_FIRE, CONDITION_ENERGY, CONDITION_BLEEDING, CONDITION_PARALYZE,
		CONDITION_DRUNK, CONDITION_DROWN, CONDITION_FREEZING, CONDITION_DAZZLED, CONDITION_CURSED, CONDITION_PACIFIED}
		for i = 1, #condition do
			if player:getCondition(condition[i]) then
				player:removeCondition(condition[i])
			end
		end

		textParam = ""
		sayParam = "Crunch."
	elseif item.itemid == 9994 then -- Roasted Dragon Wings
		local condition = Condition(CONDITION_ATTRIBUTES)
		condition:setParameter(CONDITION_PARAM_SKILL_SHIELD, 10)
		condition:setParameter(CONDITION_PARAM_TICKS, 60 * 60 * 1000)

		player:addCondition(condition)
		textParam = ""
		sayParam = "Chomp."
	elseif item.itemid == 9995 then -- Tropical Fried Terrorbird
		local condition = Condition(CONDITION_ATTRIBUTES)
		condition:setParameter(CONDITION_PARAM_STAT_MAGICPOINTS, 5)
		condition:setParameter(CONDITION_PARAM_TICKS, 60 * 60 * 1000)

		player:addCondition(condition)
		textParam = ""
		sayParam = "Chomp."
	elseif item.itemid == 9996 then -- Banana Chocolate Shake
		player:getPosition():sendMagicEffect(CONST_ME_HEARTS)
		textParam = "You don't really know what this did to you, but suddenly you feel very happy"
		sayParam = "Slurp."
	elseif item.itemid == 9997 then -- Veggie Casserole
		local condition = Condition(CONDITION_ATTRIBUTES)
		condition:setParameter(CONDITION_PARAM_TICKS, 60 * 60 * 1000)
		condition:setParameter(CONDITION_PARAM_SKILL_MELEE, 10)

		player:addCondition(condition)
		textParam = ""
		sayParam = "Yum."
	elseif item.itemid == 9998 then -- Filled Jalapeño Peppers
		local condition = Condition(CONDITION_ATTRIBUTES)
		condition:setParameter(CONDITION_PARAM_TICKS, 60 * 60 * 1000)
		condition:setParameter(CONDITION_PARAM_SPEED, 100)

		player:addCondition(condition)
		textParam = ""
		sayParam = "Munch."
	elseif item.itemid == 9999 then -- Blessed Steak
		player:addMana(math.max(1, player:getMaxMana() - player:getMana()))
		textParam = "Your mana was refilled completely."
		sayParam = "Chomp."
	elseif item.itemid == 10000 then -- Carrot Cake
		local condition = Condition(CONDITION_ATTRIBUTES)
		condition:setParameter(CONDITION_PARAM_TICKS, 60 * 60 * 1000)
		condition:setParameter(CONDITION_PARAM_SKILL_DISTANCE, 10)

		player:addCondition(condition)
		textParam = ""
		sayParam = "Mmmm."
	elseif item.itemid == 10001 then -- Northern Fishburger
		local condition = Condition(CONDITION_ATTRIBUTES)
		condition:setParameter(CONDITION_PARAM_TICKS, 60 * 60 * 1000)
		condition:setParameter(CONDITION_PARAM_SKILL_FISHING, 50)

		player:addCondition(condition)
		textParam = ""
		sayParam = "Smack."
	elseif item.itemid == 12540 then -- Coconut Shrimp Bake
		--[[
			Improve your underwater walking speed while wearing a helmet of the deep for 24 hours.
			We need to rewrite the drowning script and get the correct speed value before we can add this item.
		]]

		textParam = ""
		sayParam = "Smack."
		return false
	elseif item.itemid == 12542 then -- Pot of Blackjack
		player:addHealth(math.max(1, player:getMaxHealth() - player:getHealth()))
		local charges = item:getAttribute(ITEM_ATTRIBUTE_CHARGES)
		if charges == 1 then
			textParam = "You take the last gulp from the large bowl. No leftovers!" 
		else
			removeItem = false
			item:setAttribute(ITEM_ATTRIBUTE_CHARGES, charges - 1)
			textParam = "You take a gulp from the large bowl, but there's still some blackjack in it."
		end

		sayParam = "Gulp."
	elseif item.itemid == 12543 then -- Demonic Candy Ball
		math.randomseed(os.time())
		local number = math.random(7)
		if number == 1 then
			local condition = Condition(CONDITION_ATTRIBUTES)
			condition:setParameter(CONDITION_PARAM_TICKS, 60 * 60 * 1000)
			condition:setParameter(CONDITION_PARAM_SKILL_MELEE, 6)
		elseif number == 2 then
			local condition = Condition(CONDITION_ATTRIBUTES)
			condition:setParameter(CONDITION_PARAM_TICKS, 60 * 60 * 1000)
			condition:setParameter(CONDITION_PARAM_SKILL_DISTANCE, 6)
		elseif number == 3 then
			local condition = Condition(CONDITION_ATTRIBUTES)
			condition:setParameter(CONDITION_PARAM_SKILL_SHIELD, 6)
			condition:setParameter(CONDITION_PARAM_TICKS, 60 * 60 * 1000)
		elseif number == 4 then
			local condition = Condition(CONDITION_ATTRIBUTES)
			condition:setParameter(CONDITION_PARAM_STAT_MAGICPOINTS, 3)
			condition:setParameter(CONDITION_PARAM_TICKS, 60 * 60 * 1000)
		elseif number == 5 then
			local condition = Condition(CONDITION_HASTE)
			condition:setParameter(CONDITION_PARAM_TICKS, 22000)
			condition:setFormula(0.7, -56, 0.7, -56)
		elseif number == 6 then
			local condition = Condition(CONDITION_LIGHT)
			condition:setParameter(CONDITION_PARAM_LIGHT_LEVEL, 10)
			condition:setParameter(CONDITION_PARAM_LIGHT_COLOR, 215)
			condition:setParameter(CONDITION_PARAM_TICKS, 2 * 60 * 60 * 1000)
		elseif number == 7 then
			local condition = Condition(CONDITION_INVISIBLE)
			condition:setParameter(CONDITION_PARAM_TICKS, 10 * 60 * 1000)
		end

		player:addCondition(condition)
		textParam = ""
		sayParam = "Smack."
	elseif item.itemid == 12544 then -- Sweet Mangonaise Elixir
		local slotItem = player:getSlotItem(CONST_SLOT_RING)
		if slotItem then
			if not slotItem:hasAttribute(ITEM_ATTRIBUTE_DURATION) then
				player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "This ring cannot be multiplied.")
				return true
			end

			for i = 1, 10 do
				player:addItem(slotItem:getId(), 1)
			end
		else
			player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You need to equip a ring to use this item.")
			return true
		end

		textParam = ""
		sayParam = "Slurp!"
	end

	if textParam ~= "" then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, textParam)
	end

	if removeItem then
		item:remove(1)
	end

	player:say(sayParam, TALKTYPE_MONSTER_SAY)
	return true
end