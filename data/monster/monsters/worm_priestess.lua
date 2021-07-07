local mType = Game.createMonsterType("worm priestess")
local monster = {}

monster.name = "Worm Priestess"
monster.description = "a worm priestess"
monster.experience = 1500
monster.outfit = {
	lookType = 613,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1100
monster.maxHealth = 1100
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 23470
monster.speed = 198
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
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
	{text = "An enemy of the worm shall become his food!", yell = false},
	{text = "The great worm will swallow you!", yell = false},
	{text = "And our enemies he will swallow!", yell = false},
	{text = "The worm may guide me in this hour of darkness!", yell = false},
	{text = "From the earthy depths he comes and brings freedom!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 25},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, skill = 50, attack = 50, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 9, type = COMBAT_LIFEDRAIN, minDamage = -50, maxDamage = -130, range = 7, ShootEffect = CONST_ANI_SMALLSTONE},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ENERGYDAMAGE, minDamage = -100, maxDamage = -165, range = 4, ShootEffect = CONST_ANI_SMALLEARTH, effect = CONST_ME_POISONAREA},
	{name ="speed", interval = 2000, chance = 25, speed = {min = -900, max = -900}, duration = 10000, radius = 3, target = true, effect = CONST_ME_POFF},
	{name ="combat", interval = 2000, chance = 30, type = COMBAT_DEATHDAMAGE, minDamage = -115, maxDamage = -200, range = 7, radius = 3, target = true, ShootEffect = CONST_ANI_DEATH, effect = CONST_ME_MORTAREA},
	{name ="combat", interval = 2000, chance = 30, type = COMBAT_EARTHDAMAGE, minDamage = -200, maxDamage = -300, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_SMALLEARTH, effect = CONST_ME_HITBYPOISON}
}

monster.defenses = {
	defense = 20,
	armor = 20,
	{name ="combat", interval = 2000, chance = 11, type = COMBAT_HEALING, minDamage = 100, maxDamage = 150, effect = CONST_ME_MAGIC_RED},
	{name ="speed", interval = 2000, chance = 9, speed = {min = 198, max = 198}, duration = 1000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = 2148, chance = 100000, maxCount = 100},
	{id = 2148, chance = 60000, maxCount = 50},
	{id = 2152, chance = 41130, maxCount = 3},
	{id = 12429, chance = 15850},
	{id = 7589, chance = 12380, maxCount = 3},
	{id = 2182, chance = 11470},
	{id = 7425, chance = 5210},
	{id = 2050, chance = 4760},
	{id = 5878, chance = 4640},
	{id = 12428, chance = 2940, maxCount = 2},
	{id = 2150, chance = 2500, maxCount = 2},
	{id = 2149, chance = 2370, maxCount = 2},
	{id = 2147, chance = 2200, maxCount = 2},
	{id = 9970, chance = 2460, maxCount = 2},
	{id = 2145, chance = 2430, maxCount = 2},
	{id = 5911, chance = 1350},
	{id = 5910, chance = 1400},
	{id = 5912, chance = 1820},
	{id = 8910, chance = 1590},
	{id = 2154, chance = 560},
	{id = 2156, chance = 430},
	{id = 7401, chance = 160}
}

mType:register(monster)
