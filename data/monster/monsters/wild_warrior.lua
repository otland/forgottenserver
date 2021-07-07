local mType = Game.createMonsterType("wild warrior")
local monster = {}

monster.name = "Wild Warrior"
monster.description = "a wild warrior"
monster.experience = 60
monster.outfit = {
	lookType = 131,
	lookHead = 38,
	lookBody = 38,
	lookLegs = 38,
	lookFeet = 38,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 135
monster.maxHealth = 135
monster.runHealth = 10
monster.race = "blood"
monster.corpse = 20531
monster.speed = 190
monster.summonCost = 420
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
	pushable = true,
	boss = false,
	illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	staticAttackChance = 90,
	targetDistance = 1,
	healthHidden = false,
	canWalkOnEnergy = false,
	canWalkOnFire = false,
	canWalkOnPoison = false
}

monster.light = {
	level = 0,
	color = 0
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "An enemy!", yell = false},
	{text = "Gimme your money!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -70, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 8,
	armor = 8,
	{name ="speed", interval = 2000, chance = 15, speed = {min = 200, max = 200}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "gold coin", chance = 64000, maxCount = 30},
	{id = "axe", chance = 21000},
	{id = "brass shield", chance = 17000},
	{id = "mace", chance = 13000},
	{id = 2695, chance = 12000, maxCount = 2},
	{id = "chain helmet", chance = 5250},
	{id = "brass armor", chance = 2800},
	{id = "steel shield", chance = 1330},
	{id = "iron helmet", chance = 1000},
	{id = 2110, chance = 520}
}

mType:register(monster)
