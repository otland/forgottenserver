local mType = Game.createMonsterType("chakoya windcaller")
local monster = {}

monster.name = "Chakoya Windcaller"
monster.description = "a chakoya windcaller"
monster.experience = 48
monster.outfit = {
	lookType = 260,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 84
monster.maxHealth = 84
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 7320
monster.speed = 200
monster.summonCost = 305
monster.maxSummons = 0

monster.changeTarget = {
	interval = 60000,
	chance = 0
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
	canPushCreatures = false,
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
	{text = "Mupi! Si siyoqua jinuma!", yell = false},
	{text = "Siqsiq ji jusipa!", yell = false},
	{text = "Jagura taluka taqua!", yell = false},
	{text = "Quatu nguraka!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = -15},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 50},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 20},
	{type = COMBAT_DEATHDAMAGE , percent = -10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -22, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_ICEDAMAGE, minDamage = -16, maxDamage = -32, range = 7, ShootEffect = CONST_ANI_ICE},
	{name ="condition", interval = 2000, chance = 10, type = CONDITION_FREEZING, startDamage = 0, tick = 8000, minDamage = -130, maxDamage = -160, radius = 3, target = false, effect = CONST_ME_ICEAREA},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ICEDAMAGE, minDamage = -9, maxDamage = -30, length = 5, spread = 2, effect = CONST_ME_ICEAREA}
}

monster.defenses = {
	defense = 10,
	armor = 10,
	{name ="invisible", interval = 2000, chance = 15, duration = 3000, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 26},
	{id = "brass helmet", chance = 3250},
	{id = "bone shield", chance = 950},
	{id = 2667, chance = 29000, maxCount = 3},
	{id = "northern pike", chance = 60},
	{id = "rainbow trout", chance = 60},
	{id = "green perch", chance = 60}
}

mType:register(monster)
