local conditions = {
	CONDITION_POISON, CONDITION_FIRE, CONDITION_ENERGY,
	CONDITION_PARALYZE, CONDITION_DRUNK, CONDITION_DROWN,
	CONDITION_FREEZING, CONDITION_DAZZLED, CONDITION_CURSED,
	CONDITION_BLEEDING
}
local condition_shield = Condition(CONDITION_ATTRIBUTES)
Condition.setParameter(condition_shield, CONDITION_PARAM_SUBID, 3)
Condition.setParameter(condition_shield, CONDITION_PARAM_BUFF_SPELL, 1)
Condition.setParameter(condition_shield, CONDITION_PARAM_TICKS, 60 * 60 * 1000)
Condition.setParameter(condition_shield, CONDITION_PARAM_SKILL_SHIELD, 10)
Condition.setParameter(condition_shield, CONDITION_PARAM_FORCEUPDATE, true)

local condition_magiclevel = Condition(CONDITION_ATTRIBUTES)
Condition.setParameter(condition_magiclevel, CONDITION_PARAM_SUBID, 4)
Condition.setParameter(condition_magiclevel, CONDITION_PARAM_BUFF_SPELL, 1)
Condition.setParameter(condition_magiclevel, CONDITION_PARAM_TICKS, 60 * 60 * 1000)
Condition.setParameter(condition_magiclevel, CONDITION_PARAM_STAT_MAGICPOINTS, 5)
Condition.setParameter(condition_magiclevel, CONDITION_PARAM_FORCEUPDATE, true)

local condition_melee = Condition(CONDITION_ATTRIBUTES)
Condition.setParameter(condition_melee, CONDITION_PARAM_SUBID, 5)
Condition.setParameter(condition_melee, CONDITION_PARAM_BUFF_SPELL, 1)
Condition.setParameter(condition_melee, CONDITION_PARAM_TICKS, 60 * 60 * 1000)
Condition.setParameter(condition_melee, CONDITION_PARAM_SKILL_MELEE, 10)
Condition.setParameter(condition_melee, CONDITION_PARAM_FORCEUPDATE, true)

local condition_distance = Condition(CONDITION_ATTRIBUTES)
Condition.setParameter(condition_distance, CONDITION_PARAM_SUBID, 6)
Condition.setParameter(condition_distance, CONDITION_PARAM_BUFF_SPELL, 1)
Condition.setParameter(condition_distance, CONDITION_PARAM_TICKS, 60 * 60 * 1000)
Condition.setParameter(condition_distance, CONDITION_PARAM_SKILL_DISTANCE, 10)
Condition.setParameter(condition_distance, CONDITION_PARAM_FORCEUPDATE, true)

local condition_fishing = Condition(CONDITION_ATTRIBUTES)
Condition.setParameter(condition_fishing, CONDITION_PARAM_SUBID, 6)
Condition.setParameter(condition_fishing, CONDITION_PARAM_BUFF_SPELL, 1)
Condition.setParameter(condition_fishing, CONDITION_PARAM_TICKS, 60 * 60 * 1000)
Condition.setParameter(condition_fishing, CONDITION_PARAM_SKILL_FISHING, 50)
Condition.setParameter(condition_fishing, CONDITION_PARAM_FORCEUPDATE, true)

local condition_speed = Condition(CONDITION_HASTE)
Condition.setParameter(condition_speed, CONDITION_PARAM_TICKS, 60 * 60 * 1000)
Condition.setParameter(condition_speed, CONDITION_PARAM_SPEED, 600)

local combat_invisible = Combat()
Combat.setParameter(combat_invisible, COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_RED)
Combat.setParameter(combat_invisible, COMBAT_PARAM_AGGRESSIVE, 0)

local condition_invis = Condition(CONDITION_INVISIBLE)
Condition.setParameter(condition_invis, CONDITION_PARAM_TICKS, 600000)
Combat.addCondition(combat_invisible, condition_invis)

local condition_light_ = Condition(CONDITION_LIGHT)
condition_light_:setParameter(CONDITION_PARAM_LIGHT_LEVEL, 14)
condition_light_:setParameter(CONDITION_PARAM_LIGHT_COLOR, 154)
condition_light_:setParameter(CONDITION_PARAM_TICKS, 60 * 60 * 1000)

local helmetList = {
	[5464] = 12541, -- helmet of the deep
	[15408] = 15651 -- depth galea
}

local ringList = {
	[2164] = 2164, -- might ring
	[2202] = 2165, -- stealth ring
	[2203] = 2166, -- power ring
	[2204] = 2167, -- energy ring
	[2205] = 2168, -- life ring
	[2206] = 2169, -- time ring
	[2210] = 2207, -- sword ring
	[2211] = 2208, -- axe ring
	[2212] = 2209, -- club ring
	[2215] = 2213, -- dwarven ring
	[2216] = 2214, -- ring of healing
	[6301] = 6300, -- death ring
	[13826] = 13825, -- star ring
	[18528] = 18408, -- prismatic ring
	[26186] = 26185, -- ring of blue plasma
	[26188] = 26187, -- ring of green plasma
	[26190] = 26189 -- ring of red plasma
}

local foods = {
	[9992] = -- rotworm stew
	{ text_say = "Gulp.", text_status = "Your health has been refilled.", effect = CONST_ME_MAGIC_RED },
	[9993] = -- hydra tongue salad
	{ text_say = "Chomp.", text_status = "You feel better body condition.", effect = CONST_ME_MAGIC_RED },
	[9994] = -- roasted dragon wings
	{ text_say = "Chomp.", text_status = "You feel less vulnerable.", effect = CONST_ME_MAGIC_RED },
	[9995] = -- tropical fried terrorbird
	{ text_say = "Chomp.", text_status = "You feel smarter.", effect = CONST_ME_MAGIC_RED },
	[9996] = -- banana chocolate shake
	{ text_say = "Slurp.", text_status = "You don't really know what this did to you, but suddenly you feel very happy.", effect = CONST_ME_HEARTS },
	[9997] = -- veggie casserole
	{ text_say = "Yum.", text_status = "You feel stronger.", effect = CONST_ME_MAGIC_RED },
	[9998] = -- filled jalapeno peppers
	{ text_say = "Munch.", text_status = "Your speed has been increased.", effect = CONST_ME_MAGIC_RED },
	[9999] = -- blessed steak
	{ text_say = "Chomp.", text_status = "Your mana has been refilled.", effect = CONST_ME_MAGIC_RED },
	[10000] = -- carrot cake
	{ text_say = "Mmmm.", text_status = "You feel more focused.", effect = CONST_ME_MAGIC_RED },
	[10001] = -- northern fishburger
	{ text_say = "Smack.", text_status = "You felt fishing inspiration.", effect = CONST_ME_MAGIC_RED },
	[12540] = -- coconut shrimp bake
	{ text_say = "Yum.", text_status = "Underwater walking speed increased.", effect = CONST_ME_MAGIC_RED },
	[12542] = -- pot of blackjack
	{ text_say = "Gulp.", effect = CONST_ME_MAGIC_RED },
	[12543] = -- demonic candy ball
	{ text_say = "Smack.", effect = CONST_ME_MAGIC_RED },
	[12544] = -- sweet mangonaise elixir
	{ text_say = "Slurp!", effect = CONST_ME_MAGIC_RED }
}

local specialFoods = Action()

function specialFoods.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if player:getStorageValue(PlayerStorageKeys.specialFoods + foods[item.itemid]) > os.time() then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "You are full or you've already eaten a dish within 10 minutes.")
		return true
	end
	player:setStorageValue(PlayerStorageKeys.specialFoods + foods[item.itemid],os.time() + 10 * 60)

	local food = foods[item:getId()]

	if item.itemid == 9992 then
		item:remove(1)
		player:addHealth(player:getMaxHealth() - player:getHealth())
		player:say(food.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, food.text_status)
		fromPosition:sendMagicEffect(food.effect)
		return true
	elseif item.itemid == 9993 then
		item:remove(1)
		for i = 1, #conditions do
			player:removeCondition(conditions[i])
		end
		player:say(food.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, food.text_status)
		fromPosition:sendMagicEffect(food.effect)
		return true
	elseif item.itemid == 9994 then
		item:remove(1)
		player:addCondition(condition_shield)
		player:say(food.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, food.text_status)
		fromPosition:sendMagicEffect(food.effect)
		return true
	elseif item.itemid == 9995 then
		item:remove(1)
		player:addCondition(condition_magiclevel)
		player:say(food.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, food.text_status)
		fromPosition:sendMagicEffect(food.effect)
		return true
	elseif item.itemid == 9996 then
		item:remove(1)
		player:say(food.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, food.text_status)
		fromPosition:sendMagicEffect(food.effect)
		return true
	elseif item.itemid == 9997 then
		item:remove(1)
		player:addCondition(condition_melee)
		player:say(food.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, food.text_status)
		fromPosition:sendMagicEffect(food.effect)
		return true
	elseif item.itemid == 9998 then
		item:remove(1)
		player:addCondition(condition_speed)
		player:say(food.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, food.text_status)
		fromPosition:sendMagicEffect(food.effect)
		return true
	elseif item.itemid == 9999 then
		item:remove(1)
		player:addMana(player:getMaxMana() - player:getMana())
		player:say(food.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, food.text_status)
		fromPosition:sendMagicEffect(food.effect)
		return true
	elseif item.itemid == 10000 then
		item:remove(1)
		player:addCondition(condition_distance)
		player:say(food.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, food.text_status)
		fromPosition:sendMagicEffect(food.effect)
		return true
	elseif item.itemid == 10001 then
		item:remove(1)
		player:addCondition(condition_fishing)
		player:say(food.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, food.text_status)
		fromPosition:sendMagicEffect(food.effect)
		return true
	elseif item.itemid == 12540 then
		local playerHelmet = player:getSlotItem(CONST_SLOT_HEAD)
		if(playerHelmet.itemid == 0) then
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "You have no helmet equipped.")
			fromPosition:sendMagicEffect(CONST_ME_POFF)
			return true
		end
		if playerHelmet and helmetList[playerHelmet:getId()] then
			playerHelmet:transform(helmetList[playerHelmet:getId()])
			playerHelmet:decay()
		end
		item:remove(1)
		player:say(food.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, food.text_status)
		fromPosition:sendMagicEffect(food.effect)
		return true
	elseif item.itemid == 12542 then
		if math.random(5) == 5 then
			item:remove(1)
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "You take the last gulp from the large bowl. No leftovers!")
		else
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "You take a gulp from the large bowl, but there's still some blackjack in it.")
		end
		player:addHealth(player:getMaxHealth() - player:getHealth())
		player:say(food.text_say, TALKTYPE_MONSTER_SAY)
		fromPosition:sendMagicEffect(food.effect)
		return true
	elseif item.itemid == 12543 then
		item:remove(1)
		local bonus = { condition_shield, condition_magiclevel, condition_melee, condition_distance, condition_speed }
		local chance = math.random(4)
		if chance == 1 then
			player:addCondition(bonus[math.random(1, #bonus)])
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "You feel stronger, but you have no idea what was increased.")
		elseif chance == 2 then
			player:addCondition(condition_light_)
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "You feel enlightened.")
		elseif chance == 3 then
			player:addCondition(condition_invis)
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "You became invisible.")
		elseif chance == 4 then
			player:addHealth(player:getMaxHealth() - player:getHealth())
			player:addMana(player:getMaxMana() - player:getMana())
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "Your vitality has been restored.")
		end
			player:say(food.text_say, TALKTYPE_MONSTER_SAY)
			fromPosition:sendMagicEffect(CONST_ME_MAGIC_RED)
		return true
	elseif item.itemid == 12544 then
		local ring = player:getSlotItem(CONST_SLOT_RING)
		if(ring.itemid == 0) then
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "You have no ring equipped.")
			fromPosition:sendMagicEffect(CONST_ME_POFF)
			return true
		end
		if ringList[ring.itemid] ~= nil then
			item:remove(1)
			if ring.itemid == ringList[ring.itemid] then
				ringAmount = 20
			else
				ringAmount = 1
			end

			for i = 1, 10 do
				player:addItem(ringList[ring.itemid], ringAmount)
			end
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "Your ring has been multiplied.")
			player:say(food.text_say, TALKTYPE_MONSTER_SAY)
			return true
		else
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "This ring cannot be multiplied.")
			fromPosition:sendMagicEffect(CONST_ME_POFF)	 
		return true
		end
	end
end

for k, v in pairs(foods) do
	specialFoods:id(k)
end
specialFoods:register()
