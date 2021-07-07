local mType = Game.createMonsterType("zavarash")
local monster = {}

monster.name = "Zavarash"
monster.description = "Zavarash"
monster.experience = 21000
monster.outfit = {
	lookType = 12,
	lookHead = 0,
	lookBody = 15,
	lookLegs = 57,
	lookFeet = 84,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 35000
monster.maxHealth = 35000
monster.runHealth = 0
monster.race = "fire"
monster.corpse = 6068
monster.speed = 440
monster.summonCost = 0
monster.maxSummons = 2

monster.changeTarget = {
	interval = 2000,
	chance = 25
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
	{text = "Harrr, Harrr!", yell = true}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 90},
	{type = COMBAT_ENERGYDAMAGE, percent = 60},
	{type = COMBAT_EARTHDAMAGE, percent = 60},
	{type = COMBAT_FIREDAMAGE, percent = 70},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 60},
	{type = COMBAT_HOLYDAMAGE , percent = -20},
	{type = COMBAT_DEATHDAMAGE , percent = 65}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -600, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -6000, range = 7, ShootEffect = CONST_ANI_THROWINGKNIFE}
}

monster.defenses = {
	defense = 65,
	armor = 55,
	{name ="combat", interval = 2000, chance = 30, type = COMBAT_HEALING, minDamage = 400, maxDamage = 600, radius = 8, target = false, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 2000, chance = 18, speed = {min = 784, max = 784}, duration = 7000, effect = CONST_ME_MAGIC_GREEN},
	{name ="invisible", interval = 2000, chance = 20, duration = 10000, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Dark Torturer", chance = 100, interval = 1000}
}

monster.loot = {
	{id = 22396, chance = 100000},
	{id = 2148, chance = 50000, maxCount = 100},
	{id = 2148, chance = 50000, maxCount = 97},
	{id = 2152, chance = 100000, maxCount = 50},
	{id = 5954, chance = 100000},
	{id = 22598, chance = 60000},
	{id = 6500, chance = 60000},
	{id = 7632, chance = 40000},
	{id = 18414, chance = 40000, maxCount = 8},
	{id = 7590, chance = 34440, maxCount = 10},
	{id = 8473, chance = 34440, maxCount = 10},
	{id = 8472, chance = 34440, maxCount = 5},
	{id = 18413, chance = 30000, maxCount = 8},
	{id = 18415, chance = 30000, maxCount = 8},
	{id = 2155, chance = 20000},
	{id = 2515, chance = 15030},
	{id = 2427, chance = 15030},
	{id = 2158, chance = 15030},
	{id = 9971, chance = 15030},
	{id = 7428, chance = 15030},
	{id = 2452, chance = 12940},
	{id = 2519, chance = 11540},
	{id = 22610, chance = 10000},
	{id = 8891, chance = 9620},
	{id = 7387, chance = 8390},
	{id = 2520, chance = 6290},
	{id = 2514, chance = 1400},
	{id = 7431, chance = 700},
	{id = 8877, chance = 350},
	{id = 8928, chance = 170}
}

mType:register(monster)
