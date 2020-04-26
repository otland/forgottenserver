local mType = Game.createMonsterType("pythius the rotten")
local monster = {}

monster.name = "Pythius The Rotten"
monster.description = "Pythius the rotten"
monster.experience = 7000
monster.outfit = {
	lookType = 231,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 9500
monster.maxHealth = 9500
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 7349
monster.speed = 350
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
	{text = "YOU'LL NEVER GET MY TREASURE!", yell = true},
	{text = "MINIONS, MEET YOUR NEW BROTHER!", yell = true},
	{text = "YOU WILL REGRET THAT YOU ARE BORN!", yell = true},
	{text = "YOU MADE A HUGE WASTE!", yell = true},
	{text = "I SENSE LIFE", yell = true}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = -20},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 1950, chance = 100, minDamage = 0, maxDamage = -210, effect = CONST_ME_DRAWBLOOD},
	{name ="melee", interval = 2000, chance = 16, minDamage = 0, maxDamage = -795, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2800, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -400, range = 7, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_MORTAREA},
	{name ="combat", interval = 2000, chance = 17, type = COMBAT_EARTHDAMAGE, minDamage = -55, maxDamage = -155, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA},
	{name ="combat", interval = 2500, chance = 14, type = COMBAT_EARTHDAMAGE, minDamage = -333, maxDamage = -413, length = 8, spread = 3, effect = CONST_ME_POISONAREA},
	{name ="combat", interval = 2000, chance = 18, type = COMBAT_DEATHDAMAGE, minDamage = -165, maxDamage = -200, length = 7, spread = 0, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2500, chance = 22, type = COMBAT_MANADRAIN, minDamage = -85, maxDamage = -110, range = 8, ShootEffect = CONST_ANI_ICE},
	{name ="condition", interval = 1000, chance = 45, type = CONDITION_DROWN, startDamage = 0, tick = 5000, length = 8, spread = 3, effect = CONST_ME_POFF}
}

monster.defenses = {
	defense = 45,
	armor = 40
}

monster.summons = {
	{name = "Undead Gladiator", chance = 10, interval = 1000}
}

monster.loot = {
}

mType:register(monster)
