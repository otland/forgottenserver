local mType = Game.createMonsterType("bonebeast")
local monster = {}

monster.name = "Bonebeast"
monster.description = "a bonebeast"
monster.experience = 580
monster.outfit = {
	lookType = 101,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 515
monster.maxHealth = 515
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 6030
monster.speed = 218
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
	{text = "Cccchhhhhhhhh!", yell = false},
	{text = "Knooorrrrr!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = -25},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -200, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 100, interval = 4000}},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_EARTHDAMAGE, minDamage = -50, maxDamage = -90, range = 7, ShootEffect = CONST_ANI_POISON, effect = CONST_ME_POISONAREA},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = -25, maxDamage = -47, radius = 3, target = false, effect = CONST_ME_MAGIC_RED},
	{name ="condition", interval = 2000, chance = 10, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -50, maxDamage = -60, radius = 3, target = false, effect = CONST_ME_POISONAREA},
	{name ="condition", interval = 2000, chance = 10, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -70, maxDamage = -80, length = 6, spread = 0, effect = CONST_ME_POISONAREA},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -600, max = -600}, duration = 13000}
}

monster.defenses = {
	defense = 40,
	armor = 40,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 50, maxDamage = 60, effect = CONST_ME_HITBYPOISON}
}

monster.loot = {
	{id = 2230, chance = 49750},
	{id = "gold coin", chance = 30000, maxCount = 90},
	{id = 2229, chance = 20000},
	{id = "bony tail", chance = 9980},
	{id = "plate armor", chance = 8000},
	{id = "bone club", chance = 4950},
	{id = "bone shield", chance = 2000},
	{id = "green mushroom", chance = 1450},
	{id = "hardened bone", chance = 1000},
	{id = "health potion", chance = 540},
	{id = "bonebeast trophy", chance = 120}
}

mType:register(monster)
