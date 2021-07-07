local mType = Game.createMonsterType("troll champion")
local monster = {}

monster.name = "Troll Champion"
monster.description = "a troll champion"
monster.experience = 40
monster.outfit = {
	lookType = 281,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 75
monster.maxHealth = 75
monster.runHealth = 15
monster.race = "blood"
monster.corpse = 7926
monster.speed = 190
monster.summonCost = 340
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
	pushable = true,
	boss = false,
	illusionable = true,
	canPushItems = false,
	canPushCreatures = false,
	staticAttackChance = 95,
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
	{text = "Meee maity!", yell = false},
	{text = "Grrrr", yell = false},
	{text = "Whaaaz up!?", yell = false},
	{text = "Gruntz!", yell = false},
	{text = "Groar", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = -10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -35, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 20,
	armor = 20
}

monster.loot = {
	{id = "gold coin", chance = 64000, maxCount = 12},
	{id = "silver amulet", chance = 230},
	{id = "spear", chance = 25000},
	{id = "studded club", chance = 5450},
	{id = 2512, chance = 6000},
	{id = "arrow", chance = 5450, maxCount = 5},
	{id = "leather boots", chance = 9000},
	{id = "meat", chance = 9650},
	{id = "bunch of troll hair", chance = 3000},
	{id = "trollroot", chance = 750}
}

mType:register(monster)
