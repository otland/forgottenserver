local mType = Game.createMonsterType("muddy earth elemental")
local monster = {}

monster.name = "Muddy Earth Elemental"
monster.description = "a muddy earth elemental"
monster.experience = 450
monster.outfit = {
	lookType = 301,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 650
monster.maxHealth = 650
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 8933
monster.speed = 260
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 20000,
	chance = 50
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = false,
	boss = false,
	illusionable = false,
	canPushItems = true,
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
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 20},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -25},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 20}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -160, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_EARTHDAMAGE, minDamage = -25, maxDamage = -155, range = 7, radius = 2, target = true, effect = CONST_ME_STONES},
	{name ="condition", interval = 1000, chance = 10, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = 0, maxDamage = -26, length = 6, spread = 0, effect = CONST_ME_GROUNDSHAKER}
}

monster.defenses = {
	defense = 30,
	armor = 30
}

monster.loot = {
	{id = "small stone", chance = 40000, maxCount = 3},
	{id = "gold coin", chance = 24500, maxCount = 80},
	{id = "gold coin", chance = 24500, maxCount = 47},
	{id = 2244, chance = 22000},
	{id = "natural soil", chance = 3750}
}

mType:register(monster)
