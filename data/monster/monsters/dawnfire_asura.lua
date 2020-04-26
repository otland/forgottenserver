local mType = Game.createMonsterType("dawnfire asura")
local monster = {}

monster.name = "Dawnfire Asura"
monster.description = "a dawnfire asura"
monster.experience = 4100
monster.outfit = {
	lookType = 150,
	lookHead = 114,
	lookBody = 94,
	lookLegs = 78,
	lookFeet = 79,
	lookAddons = 1,
	lookMount = 0
}

monster.health = 2900
monster.maxHealth = 2900
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 24643
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
	illusionable = false,
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
	{text = "Encounter the flames of destiny!", yell = false},
	{text = "Fire and destruction!", yell = true},
	{text = "I will end your torment. Do not run, little mortal.", yell = false},
	{text = "Join me, brothers, for a BLOODY FRENZY!", yell = false}
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
	{id = 6558, chance = 2000},
	{id = 7590, chance = 2000},
	{id = 2150, chance = 210, maxCount = 3},
	{id = 2145, chance = 300, maxCount = 3},
	{id = 2149, chance = 300, maxCount = 3},
	{id = 2147, chance = 350, maxCount = 3},
	{id = 9970, chance = 280, maxCount = 3},
	{id = 2158, chance = 300},
	{id = 6300, chance = 260},
	{id = 6500, chance = 330},
	{id = 8871, chance = 220},
	{id = 24630, chance = 500},
	{id = 7899, chance = 100},
	{id = 2194, chance = 400},
	{id = 2663, chance = 300},
	{id = 24637, chance = 300},
	{id = 24631, chance = 500},
	{id = 5911, chance = 300},
	{id = 2133, chance = 400},
	{id = 2134, chance = 900},
	{id = 5944, chance = 300},
	{id = 8902, chance = 100},
	{id = 2187, chance = 200}
}

mType:register(monster)
