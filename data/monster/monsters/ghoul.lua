local mType = Game.createMonsterType("ghoul")
local monster = {}

monster.name = "Ghoul"
monster.description = "a ghoul"
monster.experience = 85
monster.outfit = {
	lookType = 18,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 100
monster.maxHealth = 100
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 5976
monster.speed = 144
monster.summonCost = 450
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
	pushable = false,
	boss = false,
	illusionable = true,
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
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = 20},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE , percent = -25},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -70, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = -15, maxDamage = -27, range = 1, radius = 1, target = true, effect = CONST_ME_SMALLCLOUDS}
}

monster.defenses = {
	defense = 8,
	armor = 8,
	{name ="combat", interval = 2000, chance = 5, type = COMBAT_HEALING, minDamage = 9, maxDamage = 15, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "gold coin", chance = 70000, maxCount = 30},
	{id = "rotten piece of cloth", chance = 14470},
	{id = "worm", chance = 10000, maxCount = 2},
	{id = 2050, chance = 5000},
	{id = "ghoul snack", chance = 5130},
	{id = "viking helmet", chance = 1000},
	{id = "brown piece of cloth", chance = 1000},
	{id = "pile of grave earth", chance = 950},
	{id = "scale armor", chance = 940},
	{id = 2229, chance = 280},
	{id = "life ring", chance = 190}
}

mType:register(monster)
