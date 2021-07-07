local mType = Game.createMonsterType("demon (goblin)")
local monster = {}

monster.name = "Demon"
monster.description = "a demon"
monster.experience = 25
monster.outfit = {
	lookType = 35,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 50
monster.maxHealth = 50
monster.runHealth = 35
monster.race = "blood"
monster.corpse = 6002
monster.speed = 120
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
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 20},
	{type = COMBAT_DEATHDAMAGE , percent = -10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -10, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -25, range = 7, ShootEffect = CONST_ANI_SMALLSTONE},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_MANADRAIN, range = 7},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_FIREDAMAGE, range = 7, radius = 7, target = true, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_LIFEDRAIN, length = 8, spread = 0, effect = CONST_ME_PURPLEENERGY},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_ENERGYDAMAGE, range = 1, ShootEffect = CONST_ANI_ENERGY}
}

monster.defenses = {
	defense = 6,
	armor = 6
}

monster.loot = {
	{id = "gold coin", chance = 50320, maxCount = 9},
	{id = "small stone", chance = 15290},
	{id = 2667, chance = 12750},
	{id = "small axe", chance = 9700},
	{id = "short sword", chance = 8870},
	{id = "bone club", chance = 4900},
	{id = "leather armor", chance = 2510},
	{id = "leather helmet", chance = 1940},
	{id = "dagger", chance = 1800},
	{id = 2230, chance = 1130},
	{id = "mouldy cheese", chance = 1000},
	{id = "goblin ear", chance = 940}
}

mType:register(monster)
