local mType = Game.createMonsterType("enlightened of the cult")
local monster = {}

monster.name = "Enlightened of the Cult"
monster.description = "an enlightened of the cult"
monster.experience = 500
monster.outfit = {
	lookType = 193,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 700
monster.maxHealth = 700
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 20391
monster.speed = 210
monster.summonCost = 0
monster.maxSummons = 2

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
	staticAttackChance = 50,
	targetDistance = 4,
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
	{text = "Praise to my master Urgith!", yell = false},
	{text = "You will rise as my servant!", yell = false},
	{text = "Praise to my masters! Long live the triangle!", yell = false},
	{text = "You will die in the name of the triangle!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = -5},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE , percent = 20},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -100, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 4, interval = 4000}},
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_LIFEDRAIN, minDamage = -70, maxDamage = -185, range = 1, radius = 1, target = true, ShootEffect = CONST_ANI_HOLY, effect = CONST_ME_HOLYAREA},
	{name ="drunk", interval = 2000, chance = 10, drunkenness = 25, duration = 5000, range = 7, ShootEffect = CONST_ANI_HOLY, effect = CONST_ME_HOLYDAMAGE},
	{name ="speed", interval = 2000, chance = 10, speed = {min = -360, max = -360}, duration = 6000, range = 7, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 25,
	armor = 25,
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_HEALING, minDamage = 60, maxDamage = 90, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 2000, chance = 15, duration = 4000, effect = CONST_ME_YELLOW_RINGS}
}

monster.summons = {
	{name = "Crypt Shambler", chance = 10, interval = 2000},
	{name = "Ghost", chance = 10, interval = 2000}
}

monster.loot = {
	{id = 1962, chance = 910},
	{id = "piggy bank", chance = 130},
	{id = "small sapphire", chance = 550},
	{id = "gold coin", chance = 64550, maxCount = 70},
	{id = "energy ring", chance = 450},
	{id = "platinum amulet", chance = 200},
	{id = "wand of inferno", chance = 180},
	{id = "protection amulet", chance = 790},
	{id = "skull staff", chance = 350},
	{id = "blue robe", chance = 40},
	{id = "enigmatic voodoo skull", chance = 130},
	{id = "jewelled backpack", chance = 100},
	{id = "pirate voodoo doll", chance = 430},
	{id = 6090, chance = 490},
	{id = "amber staff", chance = 100},
	{id = "strong mana potion", chance = 740},
	{id = "cultish mask", chance = 10250},
	{id = "cultish symbol", chance = 890},
	{id = "broken key ring", chance = 100}
}

mType:register(monster)
