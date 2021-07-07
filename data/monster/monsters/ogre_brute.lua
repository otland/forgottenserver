local mType = Game.createMonsterType("ogre brute")
local monster = {}

monster.name = "Ogre Brute"
monster.description = "an ogre brute"
monster.experience = 800
monster.outfit = {
	lookType = 857,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1000
monster.maxHealth = 1000
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 24799
monster.speed = 102
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
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
	{text = "You so juicy!", yell = false},
	{text = "Smash you face in!!!", yell = false},
	{text = "You stop! You lunch!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 20},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 20},
	{type = COMBAT_DEATHDAMAGE , percent = 10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, skill = 60, attack = 60, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -100, range = 7, ShootEffect = CONST_ANI_LARGEROCK},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = -60, maxDamage = -90, range = 1, radius = 3, target = false, effect = CONST_ME_GROUNDSHAKER},
	{name ="drunk", interval = 2000, chance = 10, drunkenness = 25, duration = 4000, range = 1, radius = 4, target = false, effect = CONST_ME_HITAREA},
	{name ="speed", interval = 2000, chance = 10, speed = {min = -400, max = -400}, duration = 10000, range = 4, radius = 3, target = true, ShootEffect = CONST_ANI_LARGEROCK, effect = CONST_ME_STUN}
}

monster.defenses = {
	defense = 24,
	armor = 41
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 25},
	{id = "ogre ear stud", chance = 18740},
	{id = "meat", chance = 14920, maxCount = 2},
	{id = "ogre nose ring", chance = 14660},
	{id = "strong health potion", chance = 12580},
	{id = "battle stone", chance = 4980},
	{id = "small stone", chance = 4980, maxCount = 5},
	{id = "cookie", chance = 3900, maxCount = 5},
	{id = "skull fetish", chance = 3350},
	{id = "small ruby", chance = 2460, maxCount = 2},
	{id = "club ring", chance = 2340},
	{id = "white pearl", chance = 2240},
	{id = "onyx chip", chance = 2150, maxCount = 2},
	{id = "power ring", chance = 1630},
	{id = "opal", chance = 1560, maxCount = 2},
	{id = "ogre klubba", chance = 1040},
	{id = "pot", chance = 310},
	{id = 9821, chance = 260},
	{id = "ancient stone", chance = 1900},
	{id = "bonebreaker", chance = 140},
	{id = "shamanic mask", chance = 140},
	{id = "butcher's axe", chance = 20}
}

mType:register(monster)
