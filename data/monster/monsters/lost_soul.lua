local mType = Game.createMonsterType("lost soul")
local monster = {}

monster.name = "Lost Soul"
monster.description = "a lost soul"
monster.experience = 4000
monster.outfit = {
	lookType = 232,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 5800
monster.maxHealth = 5800
monster.runHealth = 450
monster.race = "undead"
monster.corpse = 6310
monster.speed = 220
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 15
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = false,
	boss = false,
	illusionable = true,
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
	{text = "Forgive Meeeee!", yell = false},
	{text = "Mouuuurn meeee!", yell = false},
	{text = "Help meeee!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 50},
	{type = COMBAT_HOLYDAMAGE , percent = -20},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -420, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_DEATHDAMAGE, minDamage = -40, maxDamage = -210, length = 3, spread = 0, effect = CONST_ME_MAGIC_RED},
	{name ="speed", interval = 2000, chance = 20, speed = {min = -800, max = -800}, duration = 4000, radius = 6, target = false, effect = CONST_ME_SMALLCLOUDS}
}

monster.defenses = {
	defense = 30,
	armor = 30
}

monster.loot = {
	{id = "ruby necklace", chance = 1500},
	{id = "white pearl", chance = 10000, maxCount = 3},
	{id = "black pearl", chance = 12000, maxCount = 3},
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 49000, maxCount = 98},
	{id = "platinum coin", chance = 100000, maxCount = 7},
	{id = "red gem", chance = 15000},
	{id = "stone skin amulet", chance = 2780},
	{id = "blank rune", chance = 35250, maxCount = 3},
	{id = "skull staff", chance = 850},
	{id = "tower shield", chance = 740},
	{id = "skull helmet", chance = 170},
	{id = "silver goblet", chance = 4950},
	{id = "soul orb", chance = 15000},
	{id = 6300, chance = 2170},
	{id = "demonic essence", chance = 7500},
	{id = "skeleton decoration", chance = 1250},
	{id = "haunted blade", chance = 740},
	{id = "titan axe", chance = 1000},
	{id = "great mana potion", chance = 14200, maxCount = 2},
	{id = "great health potion", chance = 8800, maxCount = 2},
	{id = 9809, chance = 3500},
	{id = 9810, chance = 3500},
	{id = "unholy bone", chance = 33010}
}

mType:register(monster)
