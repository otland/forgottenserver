local mType = Game.createMonsterType("lizard chosen")
local monster = {}

monster.name = "Lizard Chosen"
monster.description = "a lizard chosen"
monster.experience = 2200
monster.outfit = {
	lookType = 344,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 3050
monster.maxHealth = 3050
monster.runHealth = 50
monster.race = "blood"
monster.corpse = 11288
monster.speed = 260
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
	staticAttackChance = 80,
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
	{text = "Grzzzzzzz!", yell = false},
	{text = "Garrrblarrrrzzzz!", yell = false},
	{text = "Kzzzzzz!", yell = false}
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
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -360, effect = CONST_ME_DRAWBLOOD},
	{name ="condition", interval = 2000, chance = 15, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -240, maxDamage = -320, length = 3, spread = 2, effect = CONST_ME_POISONAREA},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_EARTHDAMAGE, minDamage = -190, maxDamage = -340, radius = 3, target = false, effect = CONST_ME_HITBYPOISON},
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_EARTHDAMAGE, minDamage = -90, maxDamage = -180, length = 8, spread = 0, effect = CONST_ME_GREEN_RINGS}
}

monster.defenses = {
	defense = 45,
	armor = 45,
	{name ="combat", interval = 2000, chance = 10, type = COMBAT_HEALING, minDamage = 75, maxDamage = 125, effect = CONST_ME_MAGIC_GREEN}
}

monster.loot = {
	{id = "small diamond", chance = 2550, maxCount = 5},
	{id = "gold coin", chance = 33000, maxCount = 100},
	{id = "gold coin", chance = 32000, maxCount = 100},
	{id = "gold coin", chance = 32000, maxCount = 36},
	{id = "platinum coin", chance = 2920, maxCount = 5},
	{id = "tower shield", chance = 1100},
	{id = "lizard leather", chance = 2000},
	{id = "lizard scale", chance = 980, maxCount = 3},
	{id = "great health potion", chance = 5350, maxCount = 3},
	{id = "Zaoan armor", chance = 980},
	{id = "Zaoan helmet", chance = 140},
	{id = "Zaoan shoes", chance = 810},
	{id = "Zaoan legs", chance = 940},
	{id = "spiked iron ball", chance = 9890},
	{id = "corrupted flag", chance = 3350},
	{id = "cursed shoulder spikes", chance = 5800},
	{id = "scale of corruption", chance = 2870}
}

mType:register(monster)
