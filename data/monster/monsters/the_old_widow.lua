local mType = Game.createMonsterType("the old widow")
local monster = {}

monster.name = "The Old Widow"
monster.description = "The Old Widow"
monster.experience = 4200
monster.outfit = {
	lookType = 208,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 3200
monster.maxHealth = 3200
monster.runHealth = 0
monster.race = "venom"
monster.corpse = 5977
monster.speed = 240
monster.summonCost = 0
monster.maxSummons = 2

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
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 20},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -700, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 160, interval = 4000}},
	{name ="combat", interval = 1000, chance = 15, type = COMBAT_EARTHDAMAGE, minDamage = -200, maxDamage = -350, range = 7, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA},
	{name ="speed", interval = 1000, chance = 20, speed = {min = -850, max = -850}, duration = 25000, range = 7, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA},
	{name ="poisonfield", interval = 1000, chance = 10, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_POISON}
}

monster.defenses = {
	defense = 45,
	armor = 45,
	{name ="combat", interval = 1000, chance = 17, type = COMBAT_HEALING, minDamage = 225, maxDamage = 275, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 1000, chance = 8, speed = {min = 345, max = 345}, duration = 6000, effect = CONST_ME_MAGIC_RED}
}

monster.summons = {
	{name = "giant spider", chance = 13, interval = 1000}
}

monster.loot = {
	{id = "gold coin", chance = 99500, maxCount = 100},
	{id = "platinum coin", chance = 98888, maxCount = 10},
	{id = "spider silk", chance = 87000, maxCount = 3},
	{id = "steel helmet", chance = 77000},
	{id = "great health potion", chance = 67000, maxCount = 4},
	{id = "knight armor", chance = 42000},
	{id = "energy ring", chance = 33333},
	{id = "stealth ring", chance = 33333},
	{id = 2169, chance = 33333},
	{id = "knight legs", chance = 26000},
	{id = "platinum amulet", chance = 21000},
	{id = "sweet smelling bait", chance = 21000},
	{id = "spool of yarn", chance = 10000},
	{id = "dreaded cleaver", chance = 2800},
	{id = "bloody edge", chance = 1120}
}

mType:register(monster)
