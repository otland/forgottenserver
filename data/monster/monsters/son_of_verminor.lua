local mType = Game.createMonsterType("son of verminor")
local monster = {}

monster.name = "Son of Verminor"
monster.description = "Son of Verminor"
monster.experience = 5900
monster.outfit = {
	lookType = 19,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 8500
monster.maxHealth = 8500
monster.runHealth = 0
monster.race = "venom"
monster.corpse = 1490
monster.speed = 240
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
	staticAttackChance = 70,
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
	{text = "Blubb", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -473, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 450, interval = 4000}},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_EARTHDAMAGE, minDamage = -150, maxDamage = -200, range = 7, ShootEffect = CONST_ANI_POISON},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_EARTHDAMAGE, minDamage = -350, maxDamage = -390, radius = 3, target = false, effect = CONST_ME_POISONAREA},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_DEATHDAMAGE, minDamage = -220, maxDamage = -270, radius = 3, target = false, effect = CONST_ME_SMALLCLOUDS}
}

monster.defenses = {
	defense = 20,
	armor = 20,
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_HEALING, minDamage = 250, maxDamage = 350, effect = CONST_ME_MAGIC_BLUE},
	{name ="outfit", interval = 5000, chance = 10, monster = "rat", duration = 6000, effect = CONST_ME_MAGIC_BLUE},
	{name ="outfit", interval = 5000, chance = 10, monster = "larva", duration = 6000, effect = CONST_ME_MAGIC_BLUE},
	{name ="outfit", interval = 5000, chance = 10, monster = "scorpion", duration = 6000, effect = CONST_ME_MAGIC_BLUE},
	{name ="outfit", interval = 5000, chance = 10, monster = "slime", duration = 6000, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
}

mType:register(monster)
