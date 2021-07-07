local mType = Game.createMonsterType("corym charlatan")
local monster = {}

monster.name = "Corym Charlatan"
monster.description = "a corym charlatan"
monster.experience = 150
monster.outfit = {
	lookType = 532,
	lookHead = 0,
	lookBody = 78,
	lookLegs = 59,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 250
monster.maxHealth = 250
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 19725
monster.speed = 180
monster.summonCost = 490
monster.maxSummons = 0

monster.changeTarget = {
	interval = 5000,
	chance = 0
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
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
	{text = "Mehehe!", yell = false},
	{text = "Beware! Me hexing you!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 25},
	{type = COMBAT_EARTHDAMAGE, percent = 25},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = -5},
	{type = COMBAT_DEATHDAMAGE , percent = 15}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -100, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 17,
	armor = 17
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 35},
	{id = 2696, chance = 10000},
	{id = "bola", chance = 7692},
	{id = "spike shield", chance = 492},
	{id = "ratana", chance = 909},
	{id = "life preserver", chance = 854},
	{id = "cheese cutter", chance = 5882},
	{id = "cheesy figurine", chance = 518},
	{id = "earflap", chance = 7142},
	{id = "soft cheese", chance = 12500},
	{id = "rat cheese", chance = 14285},
	{id = "leather harness", chance = 537}
}

mType:register(monster)
