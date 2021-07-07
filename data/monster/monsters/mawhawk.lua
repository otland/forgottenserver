local mType = Game.createMonsterType("mawhawk")
local monster = {}

monster.name = "Mawhawk"
monster.description = "Mawhawk"
monster.experience = 14000
monster.outfit = {
	lookType = 595,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 45000
monster.maxHealth = 45000
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 22629
monster.speed = 270
monster.summonCost = 0
monster.maxSummons = 0

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
	{text = "Knorrrr!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = -10},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -450, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1800, chance = 10, type = COMBAT_EARTHDAMAGE, minDamage = -300, maxDamage = -685, length = 7, spread = 0, effect = CONST_ME_STONES},
	{name ="combat", interval = 2000, chance = 9, type = COMBAT_EARTHDAMAGE, minDamage = -250, maxDamage = -590, radius = 6, target = false, effect = CONST_ME_BIGPLANTS}
}

monster.defenses = {
	defense = 55,
	armor = 55
}

monster.loot = {
	{id = "gold coin", chance = 10000, maxCount = 100},
	{id = "platinum coin", chance = 10000, maxCount = 25},
	{id = "fire sword", chance = 10000},
	{id = "iron ore", chance = 10000},
	{id = "fish fin", chance = 10000},
	{id = "red piece of cloth", chance = 10000},
	{id = "hardened bone", chance = 10000},
	{id = "assassin dagger", chance = 10000},
	{id = "haunted blade", chance = 10000},
	{id = "nightmare blade", chance = 10000},
	{id = "violet crystal shard", chance = 10000, maxCount = 3},
	{id = "green crystal shard", chance = 10000, maxCount = 3},
	{id = "green crystal splinter", chance = 10000, maxCount = 5},
	{id = "blue crystal splinter", chance = 10000, maxCount = 5},
	{id = "cluster of solace", chance = 30000, maxCount = 2},
	{id = "frazzle tongue", chance = 30000},
	{id = "unrealized dream", chance = 30000, maxCount = 2}
}

mType:register(monster)
