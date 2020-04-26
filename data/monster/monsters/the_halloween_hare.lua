local mType = Game.createMonsterType("the halloween hare")
local monster = {}

monster.name = "The Halloween Hare"
monster.description = "The Halloween Hare"
monster.experience = 0
monster.outfit = {
	lookType = 74,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 1
monster.maxHealth = 1
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 0
monster.speed = 180
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
	chance = 95
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
	staticAttackChance = 10,
	targetDistance = 4,
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
	{type = COMBAT_PHYSICALDAMAGE, percent = 100},
	{type = COMBAT_ENERGYDAMAGE, percent = 100},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 100},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 100},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="outfit", interval = 2000, chance = 16, monster = "vampire", duration = 60000, radius = 3, target = false, effect = CONST_ME_POFF},
	{name ="outfit", interval = 2000, chance = 16, monster = "ghost", duration = 60000, radius = 3, target = false, effect = CONST_ME_POFF},
	{name ="outfit", interval = 2000, chance = 16, monster = "witch", duration = 60000, radius = 3, target = false, effect = CONST_ME_POFF},
	{name ="outfit", interval = 2000, chance = 16, monster = "skeleton", duration = 60000, radius = 3, target = false, effect = CONST_ME_POFF},
	{name ="outfit", interval = 2000, chance = 16, monster = "mummy", duration = 60000, radius = 3, target = false, effect = CONST_ME_POFF},
	{name ="outfit", interval = 2000, chance = 15, item = 2096, duration = 6000, radius = 3, target = false, effect = CONST_ME_POFF}
}

monster.defenses = {
	defense = 5,
	armor = 5
}

monster.loot = {
}

mType:register(monster)
