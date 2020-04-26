local mType = Game.createMonsterType("banshee")
local monster = {}

monster.name = "Banshee"
monster.description = "a banshee"
monster.experience = 900
monster.outfit = {
	lookType = 78,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1000
monster.maxHealth = 1000
monster.runHealth = 599
monster.race = "undead"
monster.corpse = 6019
monster.speed = 220
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
	{text = "Dance for me your dance of death!", yell = false},
	{text = "Let the music play!", yell = false},
	{text = "I will mourn your death!", yell = false},
	{text = "Are you ready to rock?", yell = false},
	{text = "Feel my gentle kiss of death.", yell = false},
	{text = "That's what I call easy listening!", yell = false},
	{text = "IIIIEEEeeeeeehhhHHHH!", yell = true}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = -25},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -100, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 3, interval = 4000}},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_LIFEDRAIN, minDamage = -100, maxDamage = -200, radius = 4, target = false, effect = CONST_ME_SOUND_RED},
	{name ="combat", interval = 2000, chance = 5, type = COMBAT_DEATHDAMAGE, minDamage = -55, maxDamage = -350, range = 1, radius = 1, target = true, effect = CONST_ME_SMALLCLOUDS},
	{name ="speed", interval = 2000, chance = 10, speed = {min = -300, max = -300}, duration = 15000, range = 7, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 25,
	armor = 25,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 120, maxDamage = 190, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "candlestick", chance = 70000},
	{id = "gold coin", chance = 30000, maxCount = 80},
	{id = "silver amulet", chance = 9400},
	{id = "simple dress", chance = 6300},
	{id = "hair of a banshee", chance = 5000},
	{id = "petrified scream", chance = 4250},
	{id = "black pearl", chance = 2130},
	{id = "silver brooch", chance = 1550},
	{id = "poison dagger", chance = 1550},
	{id = "white pearl", chance = 1110},
	{id = 2372, chance = 970},
	{id = "stone skin amulet", chance = 820},
	{id = "strong mana potion", chance = 770},
	{id = "ring of healing", chance = 750},
	{id = "blue robe", chance = 700},
	{id = "wedding ring", chance = 560},
	{id = 2175, chance = 520},
	{id = "terra mantle", chance = 340},
	{id = 2124, chance = 180},
	{id = "red robe", chance = 150},
	{id = "life crystal", chance = 100},
	{id = "sweet smelling bait", chance = 40}
}

mType:register(monster)
