local mType = Game.createMonsterType("diabolic imp")
local monster = {}

monster.name = "Diabolic Imp"
monster.description = "a diabolic imp"
monster.experience = 2900
monster.outfit = {
	lookType = 237,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1950
monster.maxHealth = 1950
monster.runHealth = 400
monster.race = "fire"
monster.corpse = 6364
monster.speed = 220
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
	{text = "He he he.", yell = false}
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
	{type = COMBAT_EARTHDAMAGE, percent = 50},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE , percent = -10},
	{type = COMBAT_DEATHDAMAGE , percent = 10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -240, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 160, interval = 4000}},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_FIREDAMAGE, minDamage = -100, maxDamage = -240, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_FIREDAMAGE, minDamage = -300, maxDamage = -430, range = 7, radius = 2, target = true, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREATTACK},
	{name ="diabolic imp skill reducer", interval = 2000, chance = 5, range = 5}
}

monster.defenses = {
	defense = 25,
	armor = 25,
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_HEALING, minDamage = 650, maxDamage = 800, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 2000, chance = 15, speed = {min = 800, max = 800}, duration = 2000, effect = CONST_ME_MAGIC_RED},
	{name ="invisible", interval = 2000, chance = 10, duration = 1000, effect = CONST_ME_TELEPORT}
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 40000, maxCount = 97},
	{id = "gold coin", chance = 3390, maxCount = 7},
	{id = "small amethyst", chance = 2250, maxCount = 3},
	{id = "stealth ring", chance = 2702},
	{id = "necrotic rod", chance = 830},
	{id = "blank rune", chance = 16666, maxCount = 2},
	{id = "double axe", chance = 1994},
	{id = 2419, chance = 5660},
	{id = "guardian shield", chance = 8130},
	{id = "pitchfork", chance = 50000},
	{id = "cleaver", chance = 8830},
	{id = "soul orb", chance = 7230},
	{id = 6300, chance = 120},
	{id = "demonic essence", chance = 8000},
	{id = "concentrated demonic blood", chance = 25000, maxCount = 2},
	{id = "magma coat", chance = 250},
	{id = "magma monocle", chance = 430}
}

mType:register(monster)
