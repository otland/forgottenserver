local mType = Game.createMonsterType("gozzler")
local monster = {}

monster.name = "Gozzler"
monster.description = "a gozzler"
monster.experience = 180
monster.outfit = {
	lookType = 313,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 240
monster.maxHealth = 240
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 9938
monster.speed = 190
monster.summonCost = 800
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
	illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 70,
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
	{text = "Huhuhuhuuu!", yell = false},
	{text = "Let the fun begin!", yell = false},
	{text = "Yihahaha!", yell = false},
	{text = "I'll bite you! Nyehehehehe!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
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
	{type = COMBAT_HOLYDAMAGE , percent = 50},
	{type = COMBAT_DEATHDAMAGE , percent = 50}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -110, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_LIFEDRAIN, minDamage = -32, maxDamage = -135, range = 1, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 15,
	armor = 15,
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_HEALING, minDamage = 30, maxDamage = 50, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 2000, chance = 15, speed = {min = 210, max = 210}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = "brown flask", chance = 8750},
	{id = "small sapphire", chance = 360},
	{id = "gold coin", chance = 52500, maxCount = 70},
	{id = "dwarven ring", chance = 190},
	{id = "battle axe", chance = 3100},
	{id = "sabre", chance = 8250},
	{id = "morning star", chance = 5000},
	{id = "serpent sword", chance = 250},
	{id = "clerical mace", chance = 900},
	{id = "plate shield", chance = 10000}
}

mType:register(monster)
