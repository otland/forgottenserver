local mType = Game.createMonsterType("hairman the huge")
local monster = {}

monster.name = "Hairman The Huge"
monster.description = "Hairman The Huge"
monster.experience = 335
monster.outfit = {
	lookType = 116,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 600
monster.maxHealth = 600
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6043
monster.speed = 230
monster.summonCost = 0
monster.maxSummons = 1

monster.changeTarget = {
	interval = 5000,
	chance = 14
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
	{text = "Hugah!", yell = false},
	{text = "Ungh! Ungh!", yell = false},
	{text = "Huaauaauaauaa!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
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
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -110, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 25,
	armor = 25,
	{name ="speed", interval = 1000, chance = 7, speed = {min = 260, max = 260}, duration = 3000, effect = CONST_ME_MAGIC_RED}
}

monster.summons = {
	{name = "Kongra", chance = 25, interval = 2000}
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 60},
	{id = "power ring", chance = 7500},
	{id = "protection amulet", chance = 3000},
	{id = "plate armor", chance = 5000},
	{id = "banana", chance = 7500, maxCount = 2},
	{id = "ape fur", chance = 2500}
}

mType:register(monster)
