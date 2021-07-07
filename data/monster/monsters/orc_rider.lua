local mType = Game.createMonsterType("orc rider")
local monster = {}

monster.name = "Orc Rider"
monster.description = "an orc rider"
monster.experience = 110
monster.outfit = {
	lookType = 4,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 180
monster.maxHealth = 180
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6010
monster.speed = 260
monster.summonCost = 490
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
	pushable = false,
	boss = false,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
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
	{text = "Grrrrrrr", yell = false},
	{text = "Orc arga Huummmak!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = -10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -130, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 9,
	armor = 9,
	{name ="speed", interval = 2000, chance = 15, speed = {min = 290, max = 290}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "gold coin", chance = 48000, maxCount = 81},
	{id = "meat", chance = 24000, maxCount = 3},
	{id = "wolf tooth chain", chance = 10210},
	{id = "orc leather", chance = 10000},
	{id = "warwolf fur", chance = 9610},
	{id = "orcish axe", chance = 7000},
	{id = "orc tooth", chance = 2000},
	{id = "battle shield", chance = 1000},
	{id = 2050, chance = 1000},
	{id = "obsidian lance", chance = 1000},
	{id = "scale armor", chance = 610}
}

mType:register(monster)
