local mType = Game.createMonsterType("kraknaknork's demon")
local monster = {}

monster.name = "Kraknaknork's Demon"
monster.description = "Kraknaknork's Demon"
monster.experience = 0
monster.outfit = {
	lookType = 12,
	lookHead = 117,
	lookBody = 58,
	lookLegs = 117,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 120
monster.maxHealth = 120
monster.runHealth = 0
monster.race = "fire"
monster.corpse = 6068
monster.speed = 400
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 5000,
	chance = 15
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = false,
	boss = true,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 85,
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
	{text = "STEP A BIT CLOSER, RIGHT THERE, HAHAHA!", yell = true},
	{text = "COME AND DIE!", yell = true},
	{text = "ROOKIE FOR BREAKFAST", yell = true}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = -8},
	{type = COMBAT_FIREDAMAGE, percent = 25},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -15},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -499, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 3000, chance = 20, type = COMBAT_FIREDAMAGE, minDamage = -10, maxDamage = -40, radius = 4, target = true, effect = CONST_ME_EXPLOSIONHIT}
}

monster.defenses = {
	defense = 5,
	armor = 10
}

monster.loot = {
	{id = "gold coin", chance = 48025, maxCount = 100},
	{id = "gold coin", chance = 48025, maxCount = 86},
	{id = "gold coin", chance = 24400}
}

mType:register(monster)
