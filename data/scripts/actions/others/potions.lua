local action = Action()

local potions = {
	[7588] = { -- strong health potion
		health = {250, 350},
		vocations = {
			VOCATION_PALADIN,
			VOCATION_KNIGHT,
			VOCATION_ROYAL_PALADIN,
			VOCATION_ELITE_KNIGHT
		},
		level = 50,
		flask = 7634,
		description = "Only knights and paladins of level 50 or above may drink this fluid."
	},
	[7589] = { -- strong mana potion
		mana = {115, 185},
		level = 50,
		flask = 7634,
		description = "Only players of level 50 or above may drink this fluid."
	},
	[7590] = { -- great mana potion
		mana = {150, 250},
		vocations = {
			VOCATION_SORCERER,
			VOCATION_DRUID,
			VOCATION_PALADIN,
			VOCATION_MASTER_SORCERER,
			VOCATION_ELDER_DRUID,
			VOCATION_ROYAL_PALADIN
		},
		level = 80,
		flask = 7635,
		description = "Only sorcerers, druids and paladins of level 80 or above may drink this fluid."
	},
	[7591] = { -- great health potion
		health = {425, 575},
		vocations = {
			VOCATION_KNIGHT,
			VOCATION_ELITE_KNIGHT
		},
		level = 80,
		flask = 7635,
		description = "Only knights of level 80 or above may drink this fluid."
	},
	[7618] = { -- health potion
		health = {125, 175},
		flask = 7636
	},
	[7620] = { -- mana potion
		mana = {75, 125},
		flask = 7636
	},
	[8472] = { -- great spirit potion
		health = {250, 350},
		mana = {100, 200},
		vocations = {
			VOCATION_PALADIN,
			VOCATION_ROYAL_PALADIN
		},
		level = 80,
		flask = 7635,
		description = "Only paladins of level 80 or above may drink this fluid."
	},
	[8473] = { -- ultimate health potion
		health = {650, 850},
		vocations = {
			VOCATION_KNIGHT,
			VOCATION_ELITE_KNIGHT
		},
		level = 130,
		flask = 7635,
		description = "Only knights of level 130 or above may drink this fluid."
	},
	[8474] = { -- antidote potion
		antidote = true,
		flask = 7636
	},
	[8704] = { -- small health potion
		health = {60, 90},
		flask = 7636
	},
	[26029] = { -- ultimate mana potion
		mana = {425, 575},
		vocations = {
			VOCATION_SORCERER,
			VOCATION_DRUID,
			VOCATION_MASTER_SORCERER,
			VOCATION_ELDER_DRUID
		},
		level = 130,
		flask = 7635,
		description = "Only druids and sorcerers of level 130 or above may drink this fluid."
	},
	[26030] = { -- ultimate spirit potion
		health = {410, 580},
		mana = {150, 250},
		vocations = {
			VOCATION_PALADIN,
			VOCATION_ROYAL_PALADIN
		},
		level = 130,
		flask = 7635,
		description = "Only paladins of level 130 or above may drink this fluid."
	},
	[26031] = { -- supreme health potion
		health = {875, 1125},
		vocations = {
			VOCATION_KNIGHT,
			VOCATION_ELITE_KNIGHT
		},
		level = 200,
		flask = 7635,
		description = "Only knights of level 200 or above may drink this fluid."
	}
}

function action.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if type(target) == "userdata" and not target:isPlayer() then
		return false
	end

	local potion = potions[item:getId()]
	if not player:getGroup():getAccess() and (potion.level and player:getLevel() < potion.level or potion.vocations and not table.contains(potion.vocations, player:getVocation():getId())) then
		player:say(potion.description, TALKTYPE_POTION)
		return true
	end

	if potion.health then
		doTargetCombat(player, target, COMBAT_HEALING, potion.health[1], potion.health[2])
	end

	if potion.mana then
		doTargetCombat(player, target, COMBAT_MANADRAIN, potion.mana[1], potion.mana[2])
	end

	if potion.antidote then
		target:removeCondition(CONDITION_POISON)
	end

	player:addAchievementProgress("Potion Addict", 100000)
	player:addItem(potion.flask)
	target:say("Aaaah...", TALKTYPE_POTION)
	target:getPosition():sendMagicEffect(CONST_ME_MAGIC_BLUE)

	if configManager.getBoolean(configKeys.REMOVE_POTION_CHARGES) then
		item:remove(1)
	end

	return true
end

for k, _ in pairs(potions) do
	action:id(k)
end
action:register()
