local mType = Game.createMonsterType("esmeralda")
local monster = {}

monster.name = "Esmeralda"
monster.description = "Esmeralda"
monster.experience = 600
monster.outfit = {
	lookType = 305,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 800
monster.maxHealth = 800
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 9871
monster.speed = 245
monster.summonCost = 0
monster.maxSummons = 0

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
	{text = "Fcccccchhhhhh", yell = false}
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
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -175, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 100, interval = 4000}},
	{name ="combat", interval = 2000, chance = 30, type = COMBAT_EARTHDAMAGE, minDamage = 0, maxDamage = -110, range = 7, ShootEffect = CONST_ANI_POISON},
	{name ="condition", interval = 2000, chance = 22, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -100, maxDamage = -100, length = 8, spread = 0, effect = CONST_ME_SMALLPLANTS},
	{name ="condition", interval = 2000, chance = 15, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -80, maxDamage = -80, radius = 3, target = false, effect = CONST_ME_HITBYPOISON},
	{name ="combat", interval = 2000, chance = 25, type = COMBAT_LIFEDRAIN, minDamage = 0, maxDamage = -110, range = 7, radius = 3, target = false, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 30,
	armor = 30
}

monster.loot = {
	{id = "gold coin", chance = 58775, maxCount = 100},
	{id = "gold coin", chance = 58775, maxCount = 19},
	{id = "platinum coin", chance = 68125, maxCount = 4},
	{id = "ring of healing", chance = 100000},
	{id = "halberd", chance = 30050},
	{id = "epee", chance = 14150},
	{id = "knight armor", chance = 45950},
	{id = "tower shield", chance = 41400},
	{id = "stone herb", chance = 9600},
	{id = "terra mantle", chance = 2775},
	{id = "mutated rat tail", chance = 100000}
}

mType:register(monster)
