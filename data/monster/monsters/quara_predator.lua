local mType = Game.createMonsterType("quara predator")
local monster = {}

monster.name = "Quara Predator"
monster.description = "a quara predator"
monster.experience = 1600
monster.outfit = {
	lookType = 20,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 2200
monster.maxHealth = 2200
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6067
monster.speed = 280
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
	{text = "Rrrah!", yell = false},
	{text = "Rraaar!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = -25},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -473, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 45,
	armor = 45,
	{name ="speed", interval = 2000, chance = 15, speed = {min = 270, max = 270}, duration = 5000, effect = CONST_ME_MAGIC_GREEN},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_HEALING, minDamage = 25, maxDamage = 75, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "small diamond", chance = 5160, maxCount = 2},
	{id = "gold coin", chance = 28000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 53},
	{id = "double axe", chance = 3171},
	{id = "shrimp", chance = 4860, maxCount = 5},
	{id = "skull helmet", chance = 400},
	{id = "fish fin", chance = 1940, maxCount = 2},
	{id = "assassin star", chance = 590},
	{id = "royal spear", chance = 9000, maxCount = 7},
	{id = "relic sword", chance = 680},
	{id = "great health potion", chance = 1000},
	{id = "glacier robe", chance = 420},
	{id = "quara bone", chance = 9090},
	{id = "giant shrimp", chance = 10}
}

mType:register(monster)
