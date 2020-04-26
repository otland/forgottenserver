local mType = Game.createMonsterType("lizard high guard")
local monster = {}

monster.name = "Lizard High Guard"
monster.description = "a lizard high guard"
monster.experience = 1450
monster.outfit = {
	lookType = 337,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1800
monster.maxHealth = 1800
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 11272
monster.speed = 240
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
	illusionable = true,
	canPushItems = true,
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
	{text = "Hizzzzzzz!", yell = false},
	{text = "To armzzzz!", yell = false},
	{text = "Engage zze aggrezzor!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 45},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -306, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 35,
	armor = 35,
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_HEALING, minDamage = 25, maxDamage = 75, effect = CONST_ME_MAGIC_GREEN}
}

monster.loot = {
	{id = "gold coin", chance = 32000, maxCount = 100},
	{id = "gold coin", chance = 32000, maxCount = 100},
	{id = "gold coin", chance = 32000, maxCount = 27},
	{id = "small emerald", chance = 2520, maxCount = 4},
	{id = "platinum coin", chance = 4900, maxCount = 2},
	{id = "tower shield", chance = 1040},
	{id = "lizard leather", chance = 1000},
	{id = "lizard scale", chance = 970},
	{id = "strong health potion", chance = 11925},
	{id = "great health potion", chance = 7070},
	{id = "red lantern", chance = 1220},
	{id = "bunch of ripe rice", chance = 4950},
	{id = "Zaoan armor", chance = 80},
	{id = "Zaoan shoes", chance = 700},
	{id = "Zaoan legs", chance = 720},
	{id = "spiked iron ball", chance = 7000},
	{id = "high guard flag", chance = 2990},
	{id = "high guard shoulderplates", chance = 8150}
}

mType:register(monster)
