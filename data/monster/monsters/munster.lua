local mType = Game.createMonsterType("munster")
local monster = {}

monster.name = "Munster"
monster.description = "Munster"
monster.experience = 35
monster.outfit = {
	lookType = 56,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 58
monster.maxHealth = 58
monster.runHealth = 10
monster.race = "blood"
monster.corpse = 2813
monster.speed = 200
monster.summonCost = 250
monster.maxSummons = 2

monster.changeTarget = {
	interval = 60000,
	chance = 0
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
	pushable = true,
	boss = true,
	illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	staticAttackChance = 80,
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
	{text = "Meeeeep!", yell = false}
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
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -15, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 4,
	armor = 2
}

monster.summons = {
	{name = "Rat", chance = 20, interval = 2000, max = 2}
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 16},
	{id = "bone club", chance = 5000},
	{id = "jacket", chance = 50000},
	{id = "cookie", chance = 5000},
	{id = 2696, chance = 50000}
}

mType:register(monster)
