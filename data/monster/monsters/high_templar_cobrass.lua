local mType = Game.createMonsterType("high templar cobrass")
local monster = {}

monster.name = "High Templar Cobrass"
monster.description = "High Templar Cobrass"
monster.experience = 515
monster.outfit = {
	lookType = 113,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 410
monster.maxHealth = 410
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 4251
monster.speed = 174
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 5000,
	chance = 0
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
	{text = "Hissss!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -80, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 26,
	armor = 26
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 100},
	{id = "lizard scale", chance = 100000},
	{id = "lizard leather", chance = 100000},
	{id = "short sword", chance = 10000},
	{id = 2376, chance = 5200},
	{id = "steel helmet", chance = 2500},
	{id = "morning star", chance = 2490},
	{id = "plate armor", chance = 1500},
	{id = "health potion", chance = 1320},
	{id = "templar scytheblade", chance = 1050},
	{id = "small emerald", chance = 760},
	{id = "salamander shield", chance = 610}
}

mType:register(monster)
