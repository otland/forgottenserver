local mType = Game.createMonsterType("thul")
local monster = {}

monster.name = "Thul"
monster.description = "Thul"
monster.experience = 2700
monster.outfit = {
	lookType = 46,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 2950
monster.maxHealth = 2950
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6065
monster.speed = 520
monster.summonCost = 670
monster.maxSummons = 2

monster.changeTarget = {
	interval = 5000,
	chance = 8
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
	pushable = false,
	boss = true,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
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
	{text = "Gaaahhh!", yell = false}
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
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -1354, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -170, radius = 3, target = false, effect = CONST_ME_HITAREA},
	{name ="poisonfield", interval = 2000, chance = 19, range = 7, radius = 3, target = false, ShootEffect = CONST_ANI_POISON},
	{name ="speed", interval = 2000, chance = 18, speed = {min = -360, max = -360}, duration = 5000, range = 7, ShootEffect = CONST_ANI_SNOWBALL},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_ICEDAMAGE, minDamage = -108, maxDamage = -137, range = 7, ShootEffect = CONST_ANI_ICE, effect = CONST_ME_ICEAREA}
}

monster.defenses = {
	defense = 15,
	armor = 15
}

monster.summons = {
	{name = "Massive Water Elementals", chance = 10, interval = 2000, max = 2}
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 77},
	{id = "shrimp", chance = 25000},
	{id = "fish fin", chance = 100000},
	{id = "relic sword", chance = 50000}
}

mType:register(monster)
