local mType = Game.createMonsterType("dworc fleshhunter")
local monster = {}

monster.name = "Dworc Fleshhunter"
monster.description = "a dworc fleshhunter"
monster.experience = 40
monster.outfit = {
	lookType = 215,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 85
monster.maxHealth = 85
monster.runHealth = 8
monster.race = "blood"
monster.corpse = 6058
monster.speed = 148
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = true,
	boss = false,
	illusionable = true,
	canPushItems = false,
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
	{text = "Grak brrretz!", yell = false},
	{text = "Grow truk grrrrr.", yell = false},
	{text = "Prek tars, dekklep zurk.", yell = false}
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
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -8},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -13}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -25, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 20, interval = 4000}},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -15, range = 7, ShootEffect = CONST_ANI_THROWINGKNIFE}
}

monster.defenses = {
	defense = 3,
	armor = 3
}

monster.loot = {
	{id = "gold coin", chance = 75000, maxCount = 13},
	{id = "leather armor", chance = 10000},
	{id = "cleaver", chance = 9000},
	{id = 2050, chance = 5500},
	{id = 2229, chance = 3300, maxCount = 3},
	{id = "poison dagger", chance = 2250},
	{id = "hunting spear", chance = 2000},
	{id = "bone shield", chance = 1000},
	{id = "tribal mask", chance = 500},
	{id = "ripper lance", chance = 100}
}

mType:register(monster)
