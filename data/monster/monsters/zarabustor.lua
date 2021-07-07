local mType = Game.createMonsterType("zarabustor")
local monster = {}

monster.name = "Zarabustor"
monster.description = "Zarabustor"
monster.experience = 8000
monster.outfit = {
	lookType = 130,
	lookHead = 0,
	lookBody = 77,
	lookLegs = 92,
	lookFeet = 115,
	lookAddons = 1,
	lookMount = 0
}

monster.health = 5100
monster.maxHealth = 5100
monster.runHealth = 900
monster.race = "blood"
monster.corpse = 20554
monster.speed = 440
monster.summonCost = 0
monster.maxSummons = 3

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
	boss = true,
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
	{text = "Killing is such a splendid diversion from my studies.", yell = false},
	{text = "Time to test my newest spells!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 95},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = -5},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -130, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_FIREDAMAGE, minDamage = 0, maxDamage = -250, range = 7, radius = 3, target = true, ShootEffect = CONST_ANI_BURSTARROW, effect = CONST_ME_FIREAREA},
	{name ="firefield", interval = 2000, chance = 10, range = 7, radius = 2, target = true, ShootEffect = CONST_ANI_FIRE},
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -250, range = 7, ShootEffect = CONST_ANI_ENERGY},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ENERGYDAMAGE, minDamage = -130, maxDamage = -350, length = 8, spread = 0, effect = CONST_ME_BIGCLOUDS},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_MANADRAIN, minDamage = 0, maxDamage = -250, range = 7},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -330, max = -330}, duration = 20000, range = 7, effect = CONST_ME_MAGIC_RED},
	{name ="warlock skill reducer", interval = 2000, chance = 5, range = 5}
}

monster.defenses = {
	defense = 20,
	armor = 20,
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_HEALING, minDamage = 100, maxDamage = 225, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 2000, chance = 20, duration = 10000, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Warlock", chance = 10, interval = 2000, max = 2},
	{name = "Green Djinn", chance = 10, interval = 2000, max = 3}
}

monster.loot = {
	{id = 2148, chance = 32000, maxCount = 80},
	{id = 2679, chance = 21000, maxCount = 4},
	{id = 2689, chance = 11000},
	{id = 2411, chance = 9600},
	{id = 2436, chance = 8330},
	{id = 7591, chance = 7190},
	{id = 7590, chance = 6760},
	{id = 7368, chance = 5500, maxCount = 4},
	{id = 2792, chance = 5000},
	{id = 2167, chance = 4200},
	{id = 2178, chance = 4000},
	{id = 2047, chance = 3500},
	{id = 2656, chance = 3390},
	{id = 2146, chance = 3190},
	{id = 2151, chance = 3160},
	{id = 7898, chance = 3040},
	{id = 2600, chance = 3000},
	{id = 2124, chance = 2670},
	{id = 12410, chance = 2500},
	{id = 2123, chance = 2420},
	{id = 2197, chance = 2320},
	{id = 1986, chance = 2310},
	{id = 2466, chance = 2240},
	{id = 2114, chance = 2060}
}

mType:register(monster)
