local mType = Game.createMonsterType("minotaur amazon")
local monster = {}

monster.name = "Minotaur Amazon"
monster.description = "a minotaur amazon"
monster.experience = 2200
monster.outfit = {
	lookType = 608,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 2600
monster.maxHealth = 2600
monster.runHealth = 240
monster.race = "blood"
monster.corpse = 23371
monster.speed = 250
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
	chance = 11
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
	{text = "I'll protect the herd!", yell = false},
	{text = "Never surrender!", yell = false},
	{text = "You won't hurt us!", yell = false},
	{text = "I will not allow your evil to continue!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = -10},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 5},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = 10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, skill = 50, attack = 50, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_MANADRAIN, minDamage = -50, maxDamage = -305, length = 8, spread = 0, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 16, type = COMBAT_LIFEDRAIN, minDamage = -50, maxDamage = -150, radius = 4, target = false, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 22, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -150, range = 7, ShootEffect = CONST_ANI_HUNTINGSPEAR, effect = CONST_ME_EXPLOSIONAREA},
	{name ="condition", interval = 2000, chance = 40, type = CONDITION_BLEEDING, startDamage = 0, tick = 4000, minDamage = -300, maxDamage = -400, radius = 4, target = true, ShootEffect = CONST_ANI_THROWINGKNIFE, effect = CONST_ME_DRAWBLOOD},
	{name ="speed", interval = 2000, chance = 25, speed = {min = -600, max = -600}, duration = 10000, radius = 1, target = true, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 35,
	armor = 35
}

monster.loot = {
	{id = 7368, chance = 3040, maxCount = 5},
	{id = 23575, chance = 19830},
	{id = 2148, chance = 100000, maxCount = 100},
	{id = 2148, chance = 60000, maxCount = 96},
	{id = 2671, chance = 60780},
	{id = 2666, chance = 4920},
	{id = 2152, chance = 62080, maxCount = 3},
	{id = 9971, chance = 290},
	{id = 5878, chance = 17950},
	{id = 7591, chance = 7670},
	{id = 7590, chance = 7380},
	{id = 12428, chance = 6800, maxCount = 2},
	{id = 2214, chance = 3040},
	{id = 2147, chance = 5500, maxCount = 2},
	{id = 9970, chance = 5500, maxCount = 2},
	{id = 2150, chance = 5070, maxCount = 2},
	{id = 2149, chance = 4050, maxCount = 2},
	{id = 23546, chance = 3040},
	{id = 23545, chance = 1740},
	{id = 5911, chance = 1010},
	{id = 2156, chance = 430},
	{id = 2197, chance = 1450},
	{id = 2154, chance = 870},
	{id = 2475, chance = 1160},
	{id = 7443, chance = 720}
}

mType:register(monster)
