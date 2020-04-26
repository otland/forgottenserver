local mType = Game.createMonsterType("azure frog")
local monster = {}

monster.name = "Azure Frog"
monster.description = "an azure frog"
monster.experience = 20
monster.outfit = {
	lookType = 226,
	lookHead = 87,
	lookBody = 85,
	lookLegs = 85,
	lookFeet = 87,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 60
monster.maxHealth = 60
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6079
monster.speed = 200
monster.summonCost = 305
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
	pushable = false,
	boss = false,
	illusionable = false,
	canPushItems = false,
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
	{text = "Ribbit! Ribbit!", yell = false},
	{text = "Ribbit!", yell = false}
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
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 10},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -24, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 5,
	armor = 5
}

monster.loot = {
	{id = "gold coin", chance = 71230, maxCount = 10},
	{id = "worm", chance = 8880}
}

mType:register(monster)
