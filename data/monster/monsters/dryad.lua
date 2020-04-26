local mType = Game.createMonsterType("dryad")
local monster = {}

monster.name = "Dryad"
monster.description = "a dryad"
monster.experience = 190
monster.outfit = {
	lookType = 137,
	lookHead = 99,
	lookBody = 41,
	lookLegs = 5,
	lookFeet = 102,
	lookAddons = 3,
	lookMount = 0
}

monster.health = 310
monster.maxHealth = 310
monster.runHealth = 30
monster.race = "blood"
monster.corpse = 20387
monster.speed = 230
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = true,
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
	{text = "Feel the wrath of mother Nature!", yell = false},
	{text = "Defiler of nature!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -20},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -90, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2500, chance = 9, type = COMBAT_EARTHDAMAGE, minDamage = -20, maxDamage = -30, radius = 4, target = false, effect = CONST_ME_BIGPLANTS},
	{name ="speed", interval = 1000, chance = 6, speed = {min = -200, max = -200}, duration = 20000, range = 7, radius = 7, target = false, effect = CONST_ME_POFF},
	{name ="drunk", interval = 1000, chance = 12, drunkenness = 25, duration = 20000, range = 7, effect = CONST_ME_HEARTS}
}

monster.defenses = {
	defense = 15,
	armor = 15,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 15, maxDamage = 60, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 2000, chance = 10, duration = 5000, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = 2787, chance = 55000, maxCount = 2},
	{id = 2148, chance = 40000, maxCount = 30},
	{id = 7732, chance = 11000, maxCount = 3},
	{id = 9928, chance = 1280},
	{id = 9931, chance = 1200},
	{id = 9929, chance = 1100},
	{id = 9927, chance = 970},
	{id = 2790, chance = 780, maxCount = 2},
	{id = 13298, chance = 210},
	{id = 2150, chance = 130}
}

mType:register(monster)
