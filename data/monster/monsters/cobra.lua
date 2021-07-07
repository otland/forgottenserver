local mType = Game.createMonsterType("cobra")
local monster = {}

monster.name = "Cobra"
monster.description = "a cobra"
monster.experience = 30
monster.outfit = {
	lookType = 81,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 65
monster.maxHealth = 65
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 3007
monster.speed = 120
monster.summonCost = 275
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
	pushable = true,
	boss = false,
	illusionable = true,
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
	{text = "Zzzzzz", yell = false},
	{text = "Fsssss", yell = false}
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
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = 0, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 100, interval = 4000}},
	{name ="condition", interval = 2000, chance = 15, type = CONDITION_POISON, startDamage = 0, tick = 4000, minDamage = -20, maxDamage = -40, range = 7, ShootEffect = CONST_ANI_POISON}
}

monster.defenses = {
	defense = 1,
	armor = 1
}

monster.loot = {
	{id = "cobra tongue", chance = 5000}
}

mType:register(monster)
