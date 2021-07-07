local mType = Game.createMonsterType("smuggler baron silvertoe")
local monster = {}

monster.name = "Smuggler Baron Silvertoe"
monster.description = "Smuggler Baron Silvertoe"
monster.experience = 170
monster.outfit = {
	lookType = 134,
	lookHead = 38,
	lookBody = 0,
	lookLegs = 94,
	lookFeet = 95,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 280
monster.maxHealth = 280
monster.runHealth = 15
monster.race = "blood"
monster.corpse = 20506
monster.speed = 400
monster.summonCost = 0
monster.maxSummons = 3

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
	boss = true,
	illusionable = false,
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
	{text = "I will make your death look like an accident!", yell = false},
	{text = "You should not have interferred with my bussiness!", yell = false},
	{text = "Bribes are expensive, murder is cheap!", yell = false},
	{text = "I see some profit in your death!", yell = false},
	{text = "I expect you to die!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = -5},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = -5}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -50, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -40, range = 7, ShootEffect = CONST_ANI_THROWINGKNIFE}
}

monster.defenses = {
	defense = 15,
	armor = 15,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 10, maxDamage = 50, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Wild Warrior", chance = 20, interval = 2000}
}

monster.loot = {
	{id = 2148, chance = 80000, maxCount = 30},
	{id = 2050, chance = 30200, maxCount = 2},
	{id = 2649, chance = 14000},
	{id = 2671, chance = 10000},
	{id = 2406, chance = 10000},
	{id = 2461, chance = 10000},
	{id = 2403, chance = 10000},
	{id = 2376, chance = 5000},
	{id = 8840, chance = 5000, maxCount = 5},
	{id = 2404, chance = 4000},
	{id = 7397, chance = 110}
}

mType:register(monster)
