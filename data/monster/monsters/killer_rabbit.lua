local mType = Game.createMonsterType("killer rabbit")
local monster = {}

monster.name = "Killer Rabbit"
monster.description = "a killer rabbit"
monster.experience = 160
monster.outfit = {
	lookType = 74,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 205
monster.maxHealth = 205
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6017
monster.speed = 300
monster.summonCost = 220
monster.maxSummons = 2

monster.changeTarget = {
	interval = 5000,
	chance = 20
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
	pushable = false,
	boss = false,
	illusionable = false,
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
	{text = "Who is lunch NOW?", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 1200, chance = 100, minDamage = 0, maxDamage = -100, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 30, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -50, range = 1}
}

monster.defenses = {
	defense = 35,
	armor = 30,
	{name ="speed", interval = 1000, chance = 40, speed = {min = 380, max = 380}, duration = 8000, effect = CONST_ME_ENERGYHIT},
	{name ="invisible", interval = 2000, chance = 30, duration = 2000, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "killer rabbit", chance = 30, interval = 2000, max = 2}
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 90}
}

mType:register(monster)
