local mType = Game.createMonsterType("apprentice sheng")
local monster = {}

monster.name = "Apprentice Sheng"
monster.description = "Apprentice Sheng"
monster.experience = 150
monster.outfit = {
	lookType = 23,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 95
monster.maxHealth = 95
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 5981
monster.speed = 170
monster.summonCost = 0
monster.maxSummons = 2

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
	illusionable = true,
	canPushItems = true,
	canPushCreatures = false,
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
	{text = "I will protect the secrets of my master!", yell = false},
	{text = "This isle will become ours alone!", yell = false},
	{text = "Kaplar!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -13, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 14, type = COMBAT_ENERGYDAMAGE, minDamage = -15, maxDamage = -25, range = 7, ShootEffect = CONST_ANI_ENERGYBALL, effect = CONST_ME_ENERGYAREA},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_FIREDAMAGE, minDamage = -15, maxDamage = -45, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_FIRE, effect = CONST_ME_FIREAREA},
	{name ="energyfield", interval = 1000, chance = 8, range = 7, radius = 1, target = true, ShootEffect = CONST_ANI_ENERGY},
	{name ="effect", interval = 10500, chance = 100, radius = 3, target = false, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 13,
	armor = 12
}

monster.summons = {
	{name = "Hyena", chance = 100, interval = 10500}
}

monster.loot = {
	{id = 2050, chance = 10000},
	{id = "gold coin", chance = 100000, maxCount = 20},
	{id = "heavy magic missile rune", chance = 2857},
	{id = "the carrot of doom", chance = 20000, maxCount = 5},
	{id = "combat knife", chance = 10000},
	{id = "leather helmet", chance = 8000},
	{id = "leather legs", chance = 8000},
	{id = 2817, chance = 10000},
	{id = "worm", chance = 50000, maxCount = 10},
	{id = "minotaur leather", chance = 100000},
	{id = "taurus mace", chance = 1000}
}

mType:register(monster)
