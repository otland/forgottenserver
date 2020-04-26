local mType = Game.createMonsterType("ferumbras mortal shell")
local monster = {}

monster.name = "Ferumbras Mortal Shell"
monster.description = "Ferumbras Mortal Shell"
monster.experience = 100000
monster.outfit = {
	lookType = 229,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 300000
monster.maxHealth = 300000
monster.runHealth = 2500
monster.race = "venom"
monster.corpse = 6078
monster.speed = 390
monster.summonCost = 0
monster.maxSummons = 3

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
	staticAttackChance = 90,
	targetDistance = 1,
	healthHidden = false,
	canWalkOnEnergy = true,
	canWalkOnFire = true,
	canWalkOnPoison = true
}

monster.light = {
	level = 0,
	color = 0
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "INSECTS!", yell = true},
	{text = "I' STILL POWERFUL ENOUGH TO CRUSH YOU!", yell = true},
	{text = "If you strike me down, I shall become more powerful than you could possibly imagine!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 65},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 65},
	{type = COMBAT_HOLYDAMAGE , percent = 65},
	{type = COMBAT_DEATHDAMAGE , percent = 65}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, skill = 90, attack = 200, effect = CONST_ME_DRAWBLOOD},
	{name ="condition", interval = 2000, chance = 20, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -250, maxDamage = -520, radius = 6, target = false, effect = CONST_ME_POISONAREA},
	{name ="combat", interval = 2000, chance = 16, type = COMBAT_MANADRAIN, minDamage = -225, maxDamage = -410, radius = 6, target = false, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_MANADRAIN, minDamage = -425, maxDamage = -810, radius = 9, target = false, effect = CONST_ME_MAGIC_BLUE},
	{name ="combat", interval = 2000, chance = 21, type = COMBAT_ENERGYDAMAGE, minDamage = -400, maxDamage = -650, radius = 9, target = false, effect = CONST_ME_ENERGYHIT},
	{name ="combat", interval = 2000, chance = 21, type = COMBAT_LIFEDRAIN, minDamage = -200, maxDamage = -450, radius = 6, target = false, effect = CONST_ME_POFF},
	{name ="combat", interval = 2000, chance = 17, type = COMBAT_LIFEDRAIN, minDamage = -590, maxDamage = -1050, length = 8, spread = 0, effect = CONST_ME_HITBYPOISON}
}

monster.defenses = {
	defense = 120,
	armor = 100,
	{name ="combat", interval = 2000, chance = 23, type = COMBAT_HEALING, minDamage = 600, maxDamage = 2490, effect = CONST_ME_MAGIC_GREEN},
	{name ="combat", interval = 2000, chance = 3, type = COMBAT_HEALING, minDamage = 20000, maxDamage = 35000, effect = CONST_ME_MAGIC_GREEN},
	{name ="speed", interval = 2000, chance = 14, speed = {min = 700, max = 700}, duration = 7000, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 2000, chance = 10, duration = 9000, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Demon", chance = 11, interval = 2000}
}

monster.loot = {
	{id = 7895, chance = 800},
	{id = 8869, chance = 400},
	{id = 2146, chance = 10000, maxCount = 10},
	{id = 7416, chance = 800},
	{id = 7427, chance = 800},
	{id = 2466, chance = 800},
	{id = 8930, chance = 400},
	{id = 25429, chance = 800},
	{id = 2148, chance = 100000, maxCount = 100},
	{id = 2149, chance = 10000, maxCount = 10},
	{id = 7632, chance = 1000},
	{id = 2156, chance = 1000},
	{id = 8868, chance = 300},
	{id = 2127, chance = 1000},
	{id = 7423, chance = 300},
	{id = 2150, chance = 10000, maxCount = 10},
	{id = 25420, chance = 800},
	{id = 7422, chance = 800},
	{id = 2143, chance = 10000, maxCount = 5},
	{id = 7418, chance = 600},
	{id = 2539, chance = 800},
	{id = 2520, chance = 800},
	{id = 25521, chance = 150},
	{id = 2148, chance = 100000, maxCount = 100},
	{id = 7896, chance = 800},
	{id = 2472, chance = 400},
	{id = 25414, chance = 100},
	{id = 7403, chance = 800},
	{id = 25522, chance = 500},
	{id = 8926, chance = 300},
	{id = 25387, chance = 3000},
	{id = 7410, chance = 800},
	{id = 2158, chance = 800},
	{id = 2152, chance = 100000, maxCount = 25},
	{id = 8928, chance = 400},
	{id = 7414, chance = 800},
	{id = 5903, chance = 100},
	{id = 25425, chance = 800},
	{id = 7382, chance = 800},
	{id = 2155, chance = 4000},
	{id = 2514, chance = 600},
	{id = 7435, chance = 800},
	{id = 25172, chance = 100000, maxCount = 3},
	{id = 2144, chance = 10000, maxCount = 5},
	{id = 2145, chance = 10000, maxCount = 10},
	{id = 25427, chance = 800},
	{id = 9970, chance = 10000, maxCount = 10},
	{id = 25423, chance = 800},
	{id = 25523, chance = 500},
	{id = 8885, chance = 800},
	{id = 2415, chance = 700},
	{id = 2522, chance = 100},
	{id = 7894, chance = 800},
	{id = 9971, chance = 800},
	{id = 7405, chance = 800},
	{id = 7411, chance = 400},
	{id = 25393, chance = 3500}
}

mType:register(monster)
