local mType = Game.createMonsterType("rift lord")
local monster = {}

monster.name = "Rift Lord"
monster.description = "a rift lord"
monster.experience = 0
monster.outfit = {
	lookType = 12,
	lookHead = 9,
	lookBody = 0,
	lookLegs = 9,
	lookFeet = 85,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 5
monster.maxHealth = 5
monster.runHealth = 0
monster.race = "fire"
monster.corpse = 6068
monster.speed = 220
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
	chance = 25
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = false,
	boss = false,
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
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = 0, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 6,
	armor = 5,
	{name ="combat", interval = 2000, chance = 19, type = COMBAT_HEALING, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
}

mType:register(monster)
