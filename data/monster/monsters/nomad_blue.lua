local mType = Game.createMonsterType("nomad blue")
local monster = {}

monster.name = "Nomad"
monster.description = "a nomad"
monster.experience = 60
monster.outfit = {
	lookType = 146,
	lookHead = 85,
	lookBody = 48,
	lookLegs = 49,
	lookFeet = 3,
	lookAddons = 3,
	lookMount = 0
}

monster.health = 160
monster.maxHealth = 160
monster.runHealth = 15
monster.race = "blood"
monster.corpse = 20603
monster.speed = 190
monster.summonCost = 420
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
	chance = 2
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
	{text = "We are the true sons of the desert!", yell = false},
	{text = "I will leave your remains to the vultures!", yell = false},
	{text = "We are swift as the wind of the desert!", yell = false},
	{text = "Your riches will be mine!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -10},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE , percent = 20},
	{type = COMBAT_DEATHDAMAGE , percent = -10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, skill = 30, attack = 40, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 6,
	armor = 5,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, effect = CONST_ME_SOUND_WHITE}
}

monster.loot = {
	{id = 2148, chance = 56210, maxCount = 40},
	{id = 8838, chance = 4870, maxCount = 3},
	{id = 12412, chance = 2140},
	{id = 2386, chance = 2730},
	{id = 2398, chance = 2120},
	{id = 12448, chance = 6420},
	{id = 2465, chance = 2350},
	{id = 2509, chance = 920},
	{id = 2459, chance = 650},
	{id = 8267, chance = 210}
}

mType:register(monster)
