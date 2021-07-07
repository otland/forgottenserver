local mType = Game.createMonsterType("azerus")
local monster = {}

monster.name = "Azerus"
monster.description = "Azerus"
monster.experience = 6000
monster.outfit = {
	lookType = 309,
	lookHead = 19,
	lookBody = 96,
	lookLegs = 21,
	lookFeet = 81,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 26000
monster.maxHealth = 26000
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 9738
monster.speed = 320
monster.summonCost = 0
monster.maxSummons = 10

monster.changeTarget = {
	interval = 5000,
	chance = 8
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
	{text = "The ultimate will finally consume this unworthy existence!", yell = false},
	{text = "My masters and I will tear down barriers and join the ultimate in its realm!", yell = false},
	{text = "The power of the Yalahari will all be mine!", yell = false},
	{text = "He who has returned from beyond has taught me secrets you can't even grasp!", yell = false},
	{text = "You can't hope to penetrate my shields!", yell = false},
	{text = "Do you really think you could beat me?", yell = false}
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
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -900, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_MANADRAIN, minDamage = 0, maxDamage = -2000, range = 7, ShootEffect = CONST_ANI_ENERGY, effect = CONST_ME_POFF},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_MANADRAIN, minDamage = 0, maxDamage = -800, range = 7, radius = 7, target = true, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_BIGCLOUDS},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_DEATHDAMAGE, minDamage = 0, maxDamage = -524, range = 7, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_MORTAREA},
	{name ="combat", interval = 1000, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = -100, maxDamage = -500, length = 8, spread = 0, effect = CONST_ME_SLEEP}
}

monster.defenses = {
	defense = 65,
	armor = 40,
	{name ="combat", interval = 1000, chance = 11, type = COMBAT_HEALING, minDamage = 400, maxDamage = 900, effect = CONST_ME_MAGIC_GREEN}
}

monster.summons = {
	{name = "Rift Worm", chance = 30, interval = 1000}
}

monster.loot = {
}

mType:register(monster)
