local mType = Game.createMonsterType("the imperor")
local monster = {}

monster.name = "The Imperor"
monster.description = "The Imperor"
monster.experience = 8000
monster.outfit = {
	lookType = 237,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 15000
monster.maxHealth = 15000
monster.runHealth = 0
monster.race = "fire"
monster.corpse = 6364
monster.speed = 330
monster.summonCost = 0
monster.maxSummons = 2

monster.changeTarget = {
	interval = 5000,
	chance = 5
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = false,
	boss = true,
	illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
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
	{text = "Muahaha!", yell = false},
	{text = "He he he!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -20},
	{type = COMBAT_HOLYDAMAGE , percent = -20},
	{type = COMBAT_DEATHDAMAGE , percent = 20}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -660, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 13, type = COMBAT_FIREDAMAGE, minDamage = -50, maxDamage = -800, range = 1, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREATTACK},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_FIREDAMAGE, minDamage = -50, maxDamage = -800, range = 7, radius = 6, target = true, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA},
	{name ="combat", interval = 2000, chance = 30, type = COMBAT_FIREDAMAGE, minDamage = -90, maxDamage = -350, length = 4, spread = 2, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA}
}

monster.defenses = {
	defense = 15,
	armor = 15,
	{name ="combat", interval = 2000, chance = 50, type = COMBAT_HEALING, minDamage = 175, maxDamage = 505, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 2000, chance = 12, speed = {min = 380, max = 380}, duration = 4000, effect = CONST_ME_MAGIC_RED},
	{name ="invisible", interval = 3000, chance = 20, duration = 5000, effect = CONST_ME_MAGIC_RED}
}

monster.summons = {
	{name = "diabolic imp", chance = 30, interval = 5500}
}

monster.loot = {
	{id = 2050, chance = 15000},
	{id = "gold coin", chance = 100000, maxCount = 79},
	{id = "small amethyst", chance = 8888},
	{id = "stealth ring", chance = 4555},
	{id = "necrotic rod", chance = 833},
	{id = "blank rune", chance = 18000},
	{id = "chain armor", chance = 7887},
	{id = "brass armor", chance = 6577},
	{id = "brass armor", chance = 6577},
	{id = "pitchfork", chance = 20000},
	{id = "soul orb", chance = 5666},
	{id = 6300, chance = 999},
	{id = "demonic essence", chance = 7777},
	{id = "infernal bolt", chance = 5000, maxCount = 5},
	{id = "the Imperor's trident", chance = 100000},
	{id = "concentrated demonic blood", chance = 2222},
	{id = "magma boots", chance = 2000},
	{id = "magma coat", chance = 2000},
	{id = "magma monocle", chance = 2000}
}

mType:register(monster)
