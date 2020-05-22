local conditions = {
	CONDITION_POISON, CONDITION_FIRE, CONDITION_ENERGY,
	CONDITION_PARALYZE, CONDITION_DRUNK, CONDITION_DROWN,
	CONDITION_FREEZING, CONDITION_DAZZLED, CONDITION_CURSED,
	CONDITION_BLEEDING
}

local condition_shield = Condition(CONDITION_ATTRIBUTES)
Condition.setParameter(condition_shield, CONDITION_PARAM_SUBID, 3)
Condition.setParameter(condition_shield, CONDITION_PARAM_BUFF_SPELL, 1)
Condition.setParameter(condition_shield, CONDITION_PARAM_TICKS, 60 * 60 * 1000) -- 1 hour
Condition.setParameter(condition_shield, CONDITION_PARAM_SKILL_SHIELD, 10)
Condition.setParameter(condition_shield, CONDITION_PARAM_FORCEUPDATE, true)

local condition_magiclevel = Condition(CONDITION_ATTRIBUTES)
Condition.setParameter(condition_magiclevel, CONDITION_PARAM_SUBID, 4)
Condition.setParameter(condition_magiclevel, CONDITION_PARAM_BUFF_SPELL, 1)
Condition.setParameter(condition_magiclevel, CONDITION_PARAM_TICKS, 60 * 60 * 1000) -- 1 hour
Condition.setParameter(condition_magiclevel, CONDITION_PARAM_STAT_MAGICPOINTS, 5)
Condition.setParameter(condition_magiclevel, CONDITION_PARAM_FORCEUPDATE, true)

local condition_melee = Condition(CONDITION_ATTRIBUTES)
Condition.setParameter(condition_melee, CONDITION_PARAM_SUBID, 5)
Condition.setParameter(condition_melee, CONDITION_PARAM_BUFF_SPELL, 1)
Condition.setParameter(condition_melee, CONDITION_PARAM_TICKS, 60 * 60 * 1000) -- 1 hour
Condition.setParameter(condition_melee, CONDITION_PARAM_SKILL_MELEE, 10)
Condition.setParameter(condition_melee, CONDITION_PARAM_FORCEUPDATE, true)

local condition_distance = Condition(CONDITION_ATTRIBUTES)
Condition.setParameter(condition_distance, CONDITION_PARAM_SUBID, 6)
Condition.setParameter(condition_distance, CONDITION_PARAM_BUFF_SPELL, 1)
Condition.setParameter(condition_distance, CONDITION_PARAM_TICKS, 60 * 60 * 1000) -- 1 hour
Condition.setParameter(condition_distance, CONDITION_PARAM_SKILL_DISTANCE, 10)
Condition.setParameter(condition_distance, CONDITION_PARAM_FORCEUPDATE, true)

local condition_fishing = Condition(CONDITION_ATTRIBUTES)
Condition.setParameter(condition_fishing, CONDITION_PARAM_SUBID, 6)
Condition.setParameter(condition_fishing, CONDITION_PARAM_BUFF_SPELL, 1)
Condition.setParameter(condition_fishing, CONDITION_PARAM_TICKS, 60 * 60 * 1000) -- 1 hour
Condition.setParameter(condition_fishing, CONDITION_PARAM_SKILL_FISHING, 50)
Condition.setParameter(condition_fishing, CONDITION_PARAM_FORCEUPDATE, true)

local condition_speed = Condition(CONDITION_HASTE)
Condition.setParameter(condition_speed, CONDITION_PARAM_TICKS, 60 * 60 * 1000) -- 1 hour
Condition.setParameter(condition_speed, CONDITION_PARAM_SPEED, 600)

local combat_invisible = Combat()
Combat.setParameter(combat_invisible, COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_RED)
Combat.setParameter(combat_invisible, COMBAT_PARAM_AGGRESSIVE, 0)

local condition_stealth = Condition(CONDITION_INVISIBLE)
Condition.setParameter(condition_stealth, CONDITION_PARAM_TICKS, 10 * 60 * 1000) -- 10 minutes
Combat.addCondition(combat_invisible, condition_stealth)

local condition_light_ = Condition(CONDITION_LIGHT)
condition_light_:setParameter(CONDITION_PARAM_LIGHT_LEVEL, 14)
condition_light_:setParameter(CONDITION_PARAM_LIGHT_COLOR, 154)
condition_light_:setParameter(CONDITION_PARAM_TICKS, 2 * 60 * 60 * 1000) -- 2 hours

local foodList = {
	[9992] = { -- rotworm stew
		text_say = "Gulp.",
		text_status = "You were healed completely.",
		effect = CONST_ME_MAGIC_GREEN
	},
	[9993] = { -- hydra tongue salad
		text_say = "Chomp.",
		text_status = "You were cured from all negative conditions.",
		effect = CONST_ME_MAGIC_GREEN
	},
	[9994] = { -- roasted dragon wings
		text_say = "Chomp.",
		text_status = "Your shielding is improved for one hour.",
		effect = CONST_ME_HITBYFIRE
	},
	[9995] = { -- tropical fried terrorbird
		text_say = "Chomp.",
		text_status = "Your magic level increases for one hour.",
		effect = CONST_ME_MAGIC_GREEN
	},
	[9996] = { -- banana chocolate shake
		text_say = "Slurp.",
		text_status = "You don't really know what this did to you, but suddenly you feel very happy.",
		effect = CONST_ME_HEARTS
	},
	[9997] = { -- veggie casserole
		text_say = "Yum.",
		text_status = "Your melee skills increase for one hour.",
		effect = CONST_ME_MAGIC_GREEN
	},
	[9998] = { -- filled jalapeno peppers
		text_say = "Munch.",
		text_status = "This hot and spicy meal makes you run really fast for one hour.",
		effect = CONST_ME_MAGIC_GREEN
	},
	[9999] = { -- blessed steak
		text_say = "Chomp.",
		text_status = "Your mana was refilled completely.",
		effect = CONST_ME_MAGIC_GREEN
	},
	[10000] = { -- carrot cake
		text_say = "Mmmm.",
		text_status = "Your distance skill is improved for the next hour.",
		effect = CONST_ME_MAGIC_GREEN
	},
	[10001] = { -- northern fishburger
		text_say = "Smack.",
		text_status = "You suddenly feel like the best fisherman in the whole world.",
		effect = CONST_ME_MAGIC_GREEN
	},
	[12540] = { -- coconut shrimp bake
		text_say = "Yum.",
		effect = CONST_ME_MAGIC_GREEN
	},
	[12542] = { -- pot of blackjack
		text_say = "Gulp.",
		effect = CONST_ME_MAGIC_GREEN
	},
	[12543] = { -- demonic candy ball
		text_say = "Smack.",
		text_status = "You're not exactly sure what it did, but you feel different. Tasted great though!",
		effect = CONST_ME_MAGIC_GREEN
	},
	[12544] = { -- sweet mangonaise elixir
		text_say = "Slurp!",
		effect = CONST_ME_MAGIC_GREEN
	}
}

local specialFoods = Action()

function specialFoods.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local foods = foodList[item:getId()]

	if player:getStorageValue(PlayerStorageKeys.specialFoods) > os.time() then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "You are full or you've already eaten a dish within 10 minutes.")
		return true
	end
	player:setStorageValue(PlayerStorageKeys.specialFoods, os.time() + 10 * 60)

	if item.itemid == 9992 then -- rotworm stew (restores full health)
		player:addHealth(player:getMaxHealth() - player:getHealth())
		item:remove(1)
		player:say(foods.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, foods.text_status)
		player:getPosition():sendMagicEffect(foods.effect)
		return true
	elseif item.itemid == 9993 then -- hydra tongue salad (remove all negative conditions)
		for i = 1, #conditions do
			player:removeCondition(conditions[i])
		end
		item:remove(1)
		player:say(foods.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, foods.text_status)
		player:getPosition():sendMagicEffect(foods.effect)
		return true
	elseif item.itemid == 9994 then -- roasted dragon wings (+10 shielding for one hour)
		player:addCondition(condition_shield)
		item:remove(1)
		player:say(foods.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, foods.text_status)
		player:getPosition():sendMagicEffect(foods.effect)
		return true
	elseif item.itemid == 9995 then -- tropical fried terrorbird (+5 magic level for one hour)
		player:addCondition(condition_magiclevel)
		item:remove(1)
		player:say(foods.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, foods.text_status)
		player:getPosition():sendMagicEffect(foods.effect)
		return true
	elseif item.itemid == 9996 then -- banana chocolate shake (do nothing)
		item:remove(1)
		player:say(foods.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, foods.text_status)
		player:getPosition():sendMagicEffect(foods.effect)
		return true
	elseif item.itemid == 9997 then -- veggie casserole (+10 melee for one hour)
		player:addCondition(condition_melee)
		item:remove(1)
		player:say(foods.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, foods.text_status)
		player:getPosition():sendMagicEffect(foods.effect)
		return true
	elseif item.itemid == 9998 then -- filled jalapeno peppers (haste for one hour)
		player:addCondition(condition_speed)
		item:remove(1)
		player:say(foods.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, foods.text_status)
		player:getPosition():sendMagicEffect(foods.effect)
		return true
	elseif item.itemid == 9999 then -- blessed steak (restores full mana)
		player:addMana(player:getMaxMana() - player:getMana())
		item:remove(1)
		player:say(foods.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, foods.text_status)
		player:getPosition():sendMagicEffect(foods.effect)
		return true
	elseif item.itemid == 10000 then -- carrot cake (+10 distance skill for one hour)
		player:addCondition(condition_distance)
		item:remove(1)
		player:say(foods.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, foods.text_status)
		player:getPosition():sendMagicEffect(foods.effect)
		return true
	elseif item.itemid == 10001 then -- northern fishburger (+50 fishing skill for one hour)
		player:addCondition(condition_fishing)
		item:remove(1)
		player:say(foods.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, foods.text_status)
		player:getPosition():sendMagicEffect(foods.effect)
		return true
	elseif item.itemid == 12542 then -- pot of blackjack
		local storage = player:getStorageValue(PlayerStorageKeys.potOfBlackjack)
		player:setStorageValue(PlayerStorageKeys.potOfBlackjack, storage + 1)
		if storage == 1 then
			item:remove(1)
			player:setStorageValue(PlayerStorageKeys.potOfBlackjack, -1)
			player:sendTextMessage(MESSAGE_STATUS_SMALL, "You take the last gulp from the large bowl. No leftovers!")
		end
		player:sendTextMessage(MESSAGE_STATUS_SMALL, "You take a gulp from the large bowl, but there's still some blackjack in it.")
		player:say(foods.text_say, TALKTYPE_MONSTER_SAY)
		player:getPosition():sendMagicEffect(foods.effect)
		player:addHealth(5000)
		return true
	elseif item.itemid == 12543 then -- demonic candy ball
		local bonus = {condition_shield, condition_magiclevel, condition_melee, condition_distance, condition_speed}
		local chance = math.random(4)
		if chance == 1 then
			player:addCondition(bonus[math.random(1, #bonus)])
		elseif chance == 2 then
			player:addCondition(condition_light_)
		elseif chance == 3 then
			player:addCondition(condition_stealth)
		elseif chance == 4 then
			player:addHealth(player:getMaxHealth() - player:getHealth())
			player:addMana(player:getMaxMana() - player:getMana())
		end
		item:remove(1)
		player:say(foods.text_say, TALKTYPE_MONSTER_SAY)
		player:sendTextMessage(MESSAGE_STATUS_SMALL, foods.text_status)
		player:getPosition():sendMagicEffect(foods.effect)
		return true
	end
end

for k, v in pairs(foodList) do
	specialFoods:id(k)
end
specialFoods:register()
