local mType = Game.createMonsterType("dworc voodoomaster")
local monster = {}

monster.name = "Dworc Voodoomaster"
monster.description = "a dworc voodoomaster"
monster.experience = 55
monster.outfit = {
	lookType = 214,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 80
monster.maxHealth = 80
monster.runHealth = 15
monster.race = "blood"
monster.corpse = 6055
monster.speed = 150
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
	pushable = true,
	boss = false,
	illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	staticAttackChance = 80,
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
	{text = "Brak brrretz!", yell = false},
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
	{type = COMBAT_FIREDAMAGE, percent = -15},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE , percent = 35},
	{type = COMBAT_DEATHDAMAGE , percent = -10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -20, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_LIFEDRAIN, minDamage = 0, maxDamage = -40, range = 1, effect = CONST_ME_MAGIC_RED},
	{name ="speed", interval = 2000, chance = 10, speed = {min = -800, max = -800}, duration = 5000, range = 7, effect = CONST_ME_MAGIC_RED},
	{name ="drunk", interval = 2000, chance = 10, drunkenness = 25, duration = 10000, range = 7, ShootEffect = CONST_ANI_ENERGY, effect = CONST_ME_TELEPORT},
	{name ="outfit", interval = 2000, chance = 10, monster = "chicken", duration = 5000, range = 7, effect = CONST_ME_MAGIC_BLUE},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_EARTHDAMAGE, minDamage = -6, maxDamage = -18, radius = 6, target = false, effect = CONST_ME_GREEN_RINGS},
	{name ="poisonfield", interval = 2000, chance = 10, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_POISON}
}

monster.defenses = {
	defense = 3,
	armor = 3,
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_HEALING, minDamage = 3, maxDamage = 9, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 2000, chance = 15, speed = {min = 200, max = 200}, duration = 4000, effect = CONST_ME_MAGIC_RED},
	{name ="invisible", interval = 2000, chance = 15, duration = 7000, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "gold coin", chance = 75000, maxCount = 17},
	{id = "leather armor", chance = 10000},
	{id = 2230, chance = 5800},
	{id = 2050, chance = 5500},
	{id = 2231, chance = 3000},
	{id = 2229, chance = 1950, maxCount = 3},
	{id = "poison dagger", chance = 1000},
	{id = "health potion", chance = 600},
	{id = "strange symbol", chance = 500},
	{id = "tribal mask", chance = 500},
	{id = 3955, chance = 130}
}

mType:register(monster)
