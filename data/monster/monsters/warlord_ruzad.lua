local mType = Game.createMonsterType("warlord ruzad")
local monster = {}

monster.name = "Warlord Ruzad"
monster.description = "Warlord Ruzad"
monster.experience = 1700
monster.outfit = {
	lookType = 2,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 2500
monster.maxHealth = 2500
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6008
monster.speed = 270
monster.summonCost = 0
monster.maxSummons = 3

monster.changeTarget = {
	interval = 2000,
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
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 30},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 80},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = -10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -300, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 35,
	armor = 32
}

monster.summons = {
	{name = "Orc Berserker", chance = 30, interval = 2000}
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 59},
	{id = "gold ring", chance = 5506},
	{id = "protection amulet", chance = 9000},
	{id = "two handed sword", chance = 18180},
	{id = "fire sword", chance = 8970},
	{id = "throwing star", chance = 6666, maxCount = 18},
	{id = "orcish axe", chance = 13000},
	{id = "plate armor", chance = 7333},
	{id = "brass legs", chance = 3333},
	{id = "crusader helmet", chance = 6670},
	{id = "meat", chance = 20000, maxCount = 3},
	{id = 2667, chance = 6667}
}

mType:register(monster)
