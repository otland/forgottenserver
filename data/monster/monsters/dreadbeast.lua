local mType = Game.createMonsterType("dreadbeast")
local monster = {}

monster.name = "Dreadbeast"
monster.description = "a dreadbeast"
monster.experience = 250
monster.outfit = {
	lookType = 101,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 800
monster.maxHealth = 800
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 6030
monster.speed = 136
monster.summonCost = 800
monster.maxSummons = 0

monster.changeTarget = {
	interval = 60000,
	chance = 10
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
	pushable = false,
	boss = false,
	illusionable = false,
	canPushItems = true,
	canPushCreatures = false,
	staticAttackChance = 80,
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
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 30},
	{type = COMBAT_ENERGYDAMAGE, percent = 15},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = 55},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 75},
	{type = COMBAT_ICEDAMAGE, percent = 40},
	{type = COMBAT_HOLYDAMAGE , percent = -50},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -50, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 100, interval = 4000}},
	{name ="combat", interval = 4000, chance = 30, type = COMBAT_DROWNDAMAGE, minDamage = -70, maxDamage = -90, range = 7, ShootEffect = CONST_ANI_SUDDENDEATH, effect = CONST_ME_LOSEENERGY},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_LIFEDRAIN, minDamage = -10, maxDamage = -80, radius = 3, target = false, effect = CONST_ME_PURPLEENERGY}
}

monster.defenses = {
	defense = 40,
	armor = 40,
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 35, maxDamage = 65, effect = CONST_ME_MAGIC_BLUE}
}

monster.loot = {
	{id = "gold coin", chance = 11690, maxCount = 88},
	{id = 2230, chance = 8230},
	{id = "plate armor", chance = 2810},
	{id = 2229, chance = 2810},
	{id = 2231, chance = 1950},
	{id = "bone club", chance = 1520},
	{id = "bone shield", chance = 1520},
	{id = "health potion", chance = 870},
	{id = "green mushroom", chance = 650},
	{id = "hardened bone", chance = 650}
}

mType:register(monster)
