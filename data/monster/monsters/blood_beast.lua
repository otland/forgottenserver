local mType = Game.createMonsterType("blood beast")
local monster = {}

monster.name = "Blood Beast"
monster.description = "a blood beast"
monster.experience = 1000
monster.outfit = {
	lookType = 602,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1600
monster.maxHealth = 1600
monster.runHealth = 0
monster.race = "venom"
monster.corpse = 23351
monster.speed = 220
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
	chance = 4
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
	{text = "Rawrrr!", yell = false},
	{text = "*grlb*", yell = false},
	{text = "Roarr!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, skill = 82, attack = 50, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 260, interval = 4000}},
	{name ="combat", interval = 2000, chance = 13, type = COMBAT_EARTHDAMAGE, minDamage = -65, maxDamage = -105, range = 5, ShootEffect = CONST_ANI_GREENSTAR, effect = CONST_ME_POISONAREA},
	{name ="condition", interval = 2000, chance = 17, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -300, maxDamage = -400, radius = 4, target = false, effect = CONST_ME_MAGIC_GREEN}
}

monster.defenses = {
	defense = 28,
	armor = 19,
	{name ="speed", interval = 2000, chance = 8, speed = {min = 314, max = 314}, duration = 6000, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 17, type = COMBAT_HEALING, minDamage = 80, maxDamage = 120, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = 23549, chance = 16010},
	{id = 10557, chance = 5080},
	{id = 23517, chance = 3720},
	{id = 2148, chance = 100000, maxCount = 100},
	{id = 2148, chance = 60000, maxCount = 50},
	{id = 23565, chance = 20040},
	{id = 7588, chance = 7710},
	{id = 7366, chance = 8290, maxCount = 5},
	{id = 23554, chance = 850},
	{id = 23550, chance = 810},
	{id = 23549, chance = 810},
	{id = 23551, chance = 1050},
	{id = 23529, chance = 780}
}

mType:register(monster)
