local mType = Game.createMonsterType("orc leader")
local monster = {}

monster.name = "Orc Leader"
monster.description = "an orc leader"
monster.experience = 270
monster.outfit = {
	lookType = 59,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 450
monster.maxHealth = 450
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6001
monster.speed = 230
monster.summonCost = 640
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
	pushable = false,
	boss = false,
	illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 90,
	targetDistance = 1,
	healthHidden = false,
	canWalkOnEnergy = false,
	canWalkOnFire = true,
	canWalkOnPoison = false
}

monster.light = {
	level = 0,
	color = 0
}

monster.voices = {
	interval = 5000,
	chance = 10,
	{text = "Ulderek futgyr human!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 20},
	{type = COMBAT_DEATHDAMAGE , percent = -10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -185, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -70, range = 7, ShootEffect = CONST_ANI_THROWINGKNIFE}
}

monster.defenses = {
	defense = 20,
	armor = 20
}

monster.loot = {
	{id = 2667, chance = 30000},
	{id = "gold coin", chance = 28000, maxCount = 35},
	{id = "orc leather", chance = 19510},
	{id = "plate shield", chance = 10000},
	{id = "throwing knife", chance = 9950, maxCount = 4},
	{id = "brown mushroom", chance = 9650},
	{id = "sword ring", chance = 3920},
	{id = "longsword", chance = 2800},
	{id = "royal spear", chance = 2600},
	{id = "brass legs", chance = 2400},
	{id = 2419, chance = 2160},
	{id = "skull belt", chance = 2100},
	{id = "plate armor", chance = 1650},
	{id = "orc tooth", chance = 1030},
	{id = "broadsword", chance = 760},
	{id = "health potion", chance = 550},
	{id = "plate legs", chance = 400},
	{id = "warrior helmet", chance = 180}
}

mType:register(monster)
