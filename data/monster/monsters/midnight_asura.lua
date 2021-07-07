local mType = Game.createMonsterType("midnight asura")
local monster = {}

monster.name = "Midnight Asura"
monster.description = "a midnight asura"
monster.experience = 4100
monster.outfit = {
	lookType = 150,
	lookHead = 0,
	lookBody = 114,
	lookLegs = 90,
	lookFeet = 90,
	lookAddons = 1,
	lookMount = 0
}

monster.health = 3100
monster.maxHealth = 3100
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 24644
monster.speed = 210
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
	illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 80,
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
	{text = "Ah, the sweet music of a beating heart!", yell = false},
	{text = "Death and Darkness", yell = false},
	{text = "Embrace the night!", yell = false},
	{text = "May night fall upon you!", yell = false}
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
	{type = COMBAT_EARTHDAMAGE, percent = -15},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -269, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_MANADRAIN, minDamage = 0, maxDamage = -70, range = 7},
	{name ="firefield", interval = 2000, chance = 10, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_FIRE},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = -50, maxDamage = -200, length = 8, spread = 0, effect = CONST_ME_PURPLEENERGY},
	{name ="energy strike", interval = 2000, chance = 10, minDamage = -10, maxDamage = -100, range = 1},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -100, max = -100}, duration = 30000, radius = 1, target = true, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 55,
	armor = 55,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 50, maxDamage = 100, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 2000, chance = 15, speed = {min = 320, max = 320}, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.loot = {
	{id = 2148, chance = 100000, maxCount = 100},
	{id = 2148, chance = 80000, maxCount = 100},
	{id = 2148, chance = 60000, maxCount = 42},
	{id = 2152, chance = 18200, maxCount = 6},
	{id = 7368, chance = 2000, maxCount = 2},
	{id = 2144, chance = 2000},
	{id = 6558, chance = 2000},
	{id = 6500, chance = 2210},
	{id = 2145, chance = 800, maxCount = 3},
	{id = 2149, chance = 900, maxCount = 3},
	{id = 2147, chance = 650, maxCount = 3},
	{id = 2146, chance = 580, maxCount = 3},
	{id = 9970, chance = 580, maxCount = 3},
	{id = 7591, chance = 700},
	{id = 2143, chance = 560},
	{id = 7404, chance = 130},
	{id = 2158, chance = 320},
	{id = 2656, chance = 300},
	{id = 9971, chance = 300},
	{id = 24630, chance = 300},
	{id = 2186, chance = 300},
	{id = 2185, chance = 90},
	{id = 24637, chance = 80},
	{id = 24631, chance = 200},
	{id = 8889, chance = 90},
	{id = 2134, chance = 100},
	{id = 2170, chance = 100},
	{id = 5944, chance = 100},
	{id = 8902, chance = 100},
	{id = 3967, chance = 100},
	{id = 8910, chance = 70},
	{id = 2154, chance = 100}
}

mType:register(monster)
