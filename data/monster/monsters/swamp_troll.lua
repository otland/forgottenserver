local mType = Game.createMonsterType("swamp troll")
local monster = {}

monster.name = "Swamp Troll"
monster.description = "a swamp troll"
monster.experience = 25
monster.outfit = {
	lookType = 76,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 55
monster.maxHealth = 55
monster.runHealth = 15
monster.race = "blood"
monster.corpse = 6018
monster.speed = 128
monster.summonCost = 320
monster.maxSummons = 0

monster.changeTarget = {
	interval = 5000,
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
	{text = "Grrrr", yell = false},
	{text = "Groar!", yell = false},
	{text = "Me strong! Me ate spinach!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 15},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -13, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 1, interval = 4000}}
}

monster.defenses = {
	defense = 6,
	armor = 6
}

monster.loot = {
	{id = 2667, chance = 60000},
	{id = "gold coin", chance = 50300, maxCount = 5},
	{id = 2050, chance = 15000},
	{id = "spear", chance = 13000},
	{id = "mouldy cheese", chance = 10000},
	{id = "leather boots", chance = 10000},
	{id = "swamp grass", chance = 3100},
	{id = "medicine pouch", chance = 2400},
	{id = 5901, chance = 2000},
	{id = "troll green", chance = 1000},
	{id = "fishing rod", chance = 80}
}

mType:register(monster)
