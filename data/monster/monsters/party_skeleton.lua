local mType = Game.createMonsterType("party skeleton")
local monster = {}

monster.name = "Party Skeleton"
monster.description = "a party skeleton"
monster.experience = 0
monster.outfit = {
	lookType = 33,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 40
monster.maxHealth = 40
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 5972
monster.speed = 154
monster.summonCost = 300
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
	chance = 0
}

monster.flags = {
	summonable = true,
	attackable = false,
	hostile = false,
	convinceable = true,
	pushable = true,
	boss = false,
	illusionable = false,
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
	{type = COMBAT_HOLYDAMAGE , percent = -25},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -20, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 9, type = COMBAT_LIFEDRAIN, minDamage = -7, maxDamage = -13, range = 1}
}

monster.defenses = {
	defense = 9,
	armor = 2
}

monster.loot = {
	{id = 2050, chance = 25000},
	{id = 2050, chance = 25000},
	{id = "gold coin", chance = 24500, maxCount = 10},
	{id = 2230, chance = 49750},
	{id = 2376, chance = 1500},
	{id = "hatchet", chance = 23750},
	{id = "mace", chance = 19500},
	{id = "viking helmet", chance = 8250},
	{id = "brass shield", chance = 13250}
}

mType:register(monster)
