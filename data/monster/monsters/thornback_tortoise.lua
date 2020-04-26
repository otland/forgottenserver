local mType = Game.createMonsterType("thornback tortoise")
local monster = {}

monster.name = "Thornback Tortoise"
monster.description = "a thornback tortoise"
monster.experience = 150
monster.outfit = {
	lookType = 198,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 300
monster.maxHealth = 300
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6073
monster.speed = 200
monster.summonCost = 490
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
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 45},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -110, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 40, interval = 4000}}
}

monster.defenses = {
	defense = 40,
	armor = 40
}

monster.loot = {
	{id = "white pearl", chance = 1600},
	{id = "black pearl", chance = 800},
	{id = "gold coin", chance = 89500, maxCount = 48},
	{id = "war hammer", chance = 260},
	{id = 2667, chance = 10800, maxCount = 2},
	{id = "white mushroom", chance = 1200},
	{id = "brown mushroom", chance = 700},
	{id = "tortoise egg", chance = 790, maxCount = 3},
	{id = "turtle shell", chance = 800},
	{id = "health potion", chance = 1600},
	{id = "thorn", chance = 15980}
}

mType:register(monster)
