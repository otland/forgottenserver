local mType = Game.createMonsterType("eternal guardian")
local monster = {}

monster.name = "Eternal Guardian"
monster.description = "an eternal guardian"
monster.experience = 1800
monster.outfit = {
	lookType = 345,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 2500
monster.maxHealth = 2500
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 11300
monster.speed = 180
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
	{text = "Zzrkrrch!", yell = false},
	{text = "<crackle>", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 25},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 70},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE , percent = 20},
	{type = COMBAT_DEATHDAMAGE , percent = 20}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -300, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 40,
	armor = 40
}

monster.loot = {
	{id = "small stone", chance = 30230, maxCount = 10},
	{id = "gold coin", chance = 99930, maxCount = 100},
	{id = "platinum coin", chance = 99540, maxCount = 4},
	{id = "guardian halberd", chance = 560},
	{id = "tower shield", chance = 820},
	{id = "iron ore", chance = 1700},
	{id = "ancient stone", chance = 20020},
	{id = "shiny stone", chance = 800},
	{id = "Zaoan sword", chance = 100},
	{id = "Zaoan halberd", chance = 1860},
	{id = "spiked iron ball", chance = 9960},
	{id = "clay lump", chance = 720},
	{id = "piece of marble rock", chance = 430},
	{id = 13757, chance = 400}
}

mType:register(monster)
