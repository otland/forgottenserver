local mType = Game.createMonsterType("kreebosh the exile")
local monster = {}

monster.name = "Kreebosh the Exile"
monster.description = "Kreebosh the Exile"
monster.experience = 350
monster.outfit = {
	lookType = 103,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 705
monster.maxHealth = 705
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 7349
monster.speed = 270
monster.summonCost = 0
monster.maxSummons = 2

monster.changeTarget = {
	interval = 0,
	chance = 0
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
	canPushCreatures = false,
	staticAttackChance = 95,
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
	{text = "I bet you wish you weren't here.", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 55},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -100, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 6000, chance = 80, type = COMBAT_FIREDAMAGE, minDamage = 0, maxDamage = -120, radius = 3, target = false, effect = CONST_ME_ENERGYHIT},
	{name ="speed", interval = 3500, chance = 35, speed = {min = -450, max = -450}, duration = 20000, range = 5, radius = 1, target = true, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 6000, chance = 40, type = COMBAT_PHYSICALDAMAGE, minDamage = -20, maxDamage = -100, range = 5, radius = 1, target = true, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_MORTAREA},
	{name ="combat", interval = 5000, chance = 20, type = COMBAT_ENERGYDAMAGE, minDamage = -40, maxDamage = -200, range = 5, radius = 1, target = true, ShootEffect = CONST_ANI_ENERGY, effect = CONST_ME_FIREAREA},
	{name ="drunk", interval = 1000, chance = 20, drunkenness = 25, duration = 30000, range = 5, radius = 1, target = true},
	{name ="outfit", interval = 2000, chance = 50, monster = "Rat", duration = 60000, range = 5, radius = 1, target = true, effect = CONST_ME_MAGIC_GREEN}
}

monster.defenses = {
	defense = 40,
	armor = 30
}

monster.summons = {
	{name = "Green Djinn", chance = 20, interval = 5000}
}

monster.loot = {
}

mType:register(monster)
