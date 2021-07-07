local mType = Game.createMonsterType("darakan the executioner")
local monster = {}

monster.name = "Darakan the Executioner"
monster.description = "Darakan the Executioner"
monster.experience = 1600
monster.outfit = {
	lookType = 255,
	lookHead = 78,
	lookBody = 95,
	lookLegs = 0,
	lookFeet = 95,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 3480
monster.maxHealth = 3480
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 7349
monster.speed = 205
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 0,
	chance = 0
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
	staticAttackChance = 95,
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
	{text = "FIGHT LIKE A BARBARIAN!", yell = true},
	{text = "VICTORY IS MINE!", yell = true},
	{text = "I AM your father!", yell = false},
	{text = "To be the man you have to beat the man!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = -15},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 15},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -210, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1000, chance = 100, type = COMBAT_PHYSICALDAMAGE, minDamage = -72, maxDamage = -130, ShootEffect = CONST_ANI_SPEAR}
}

monster.defenses = {
	defense = 31,
	armor = 30,
	{name ="combat", interval = 6000, chance = 65, type = COMBAT_HEALING, minDamage = 20, maxDamage = 50, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
}

mType:register(monster)
