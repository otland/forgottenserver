local mType = Game.createMonsterType("robby the reckless")
local monster = {}

monster.name = "Robby the Reckless"
monster.description = "Robby the Reckless"
monster.experience = 110
monster.outfit = {
	lookType = 129,
	lookHead = 76,
	lookBody = 115,
	lookLegs = 99,
	lookFeet = 114,
	lookAddons = 1,
	lookMount = 0
}

monster.health = 155
monster.maxHealth = 155
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20498
monster.speed = 220
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 5
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
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -80, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 25,
	armor = 25
}

monster.loot = {
	{id = "bag with stolen gold", chance = 100000},
	{id = "platinum coin", chance = 100000, maxCount = 93}
}

mType:register(monster)
