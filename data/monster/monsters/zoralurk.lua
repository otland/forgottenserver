local mType = Game.createMonsterType("zoralurk")
local monster = {}

monster.name = "Zoralurk"
monster.description = "Zoralurk"
monster.experience = 30000
monster.outfit = {
	lookType = 12,
	lookHead = 0,
	lookBody = 98,
	lookLegs = 86,
	lookFeet = 94,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 55000
monster.maxHealth = 55000
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 6068
monster.speed = 400
monster.summonCost = 0
monster.maxSummons = 2

monster.changeTarget = {
	interval = 10000,
	chance = 20
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
	staticAttackChance = 98,
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
	{text = "I AM ZORALURK, THE DEMON WITH A THOUSAND FACES!", yell = true},
	{text = "BRING IT, COCKROACHES!", yell = true}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -1013, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 12, type = COMBAT_ENERGYDAMAGE, minDamage = -600, maxDamage = -900, radius = 7, target = false, effect = CONST_ME_ENERGYHIT},
	{name ="combat", interval = 1000, chance = 12, type = COMBAT_EARTHDAMAGE, minDamage = -400, maxDamage = -800, radius = 7, target = false, effect = CONST_ME_SMALLPLANTS},
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_MANADRAIN, minDamage = -500, maxDamage = -800, range = 7, effect = CONST_ME_MAGIC_BLUE},
	{name ="combat", interval = 3000, chance = 35, type = COMBAT_FIREDAMAGE, minDamage = -200, maxDamage = -600, range = 7, radius = 7, target = true, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA}
}

monster.defenses = {
	defense = 65,
	armor = 55,
	{name ="combat", interval = 2000, chance = 35, type = COMBAT_HEALING, minDamage = 300, maxDamage = 800, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 4000, chance = 80, speed = {min = 440, max = 440}, duration = 6000, effect = CONST_ME_MAGIC_RED},
	{name ="outfit", interval = 2000, chance = 10, monster = "behemoth", duration = 10000, effect = CONST_ME_CRAPS},
	{name ="outfit", interval = 2000, chance = 10, monster = "fire devil", duration = 10000, effect = CONST_ME_CRAPS},
	{name ="outfit", interval = 2000, chance = 10, monster = "giant spider", duration = 10000, effect = CONST_ME_CRAPS},
	{name ="outfit", interval = 2000, chance = 10, monster = "undead dragon", duration = 10000, effect = CONST_ME_CRAPS},
	{name ="outfit", interval = 2000, chance = 10, monster = "lost soul", duration = 10000, effect = CONST_ME_CRAPS}
}

monster.summons = {
	{name = "demon", chance = 50, interval = 4000}
}

monster.loot = {
	{id = "white pearl", chance = 10000, maxCount = 5},
	{id = "gold coin", chance = 100000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 90},
	{id = "boots of haste", chance = 16033},
	{id = "giant sword", chance = 60000},
	{id = "bright sword", chance = 20000},
	{id = "bright sword", chance = 20000},
	{id = "warlord sword", chance = 6000},
	{id = "patched boots", chance = 7000},
	{id = 6530, chance = 16000}
}

mType:register(monster)
