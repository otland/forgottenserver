local mType = Game.createMonsterType("elder forest fury")
local monster = {}

monster.name = "Elder Forest Fury"
monster.description = "an elder forest fury"
monster.experience = 330
monster.outfit = {
	lookType = 569,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 670
monster.maxHealth = 670
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 21359
monster.speed = 240
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 10
}

monster.flags = {
	summonable = false,
	attackable = true,
	hostile = true,
	convinceable = false,
	pushable = false,
	boss = false,
	illusionable = true,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 80,
	targetDistance = 3,
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
	{text = "To arms, sisters!", yell = false},
	{text = "Feel the wrath of mother forest!", yell = false},
	{text = "By the power of Greenskull!", yell = false}
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
	{type = COMBAT_FIREDAMAGE, percent = -5},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = 40}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -53, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 5, interval = 4000}},
	{name ="combat", interval = 1500, chance = 40, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -100, range = 5, radius = 4, target = true, ShootEffect = CONST_ANI_HUNTINGSPEAR, effect = CONST_ME_MAGIC_GREEN}
}

monster.defenses = {
	defense = 25,
	armor = 25
}

monster.loot = {
	{id = "gold coin", chance = 90000, maxCount = 68},
	{id = "bolt", chance = 45000, maxCount = 15},
	{id = "hunting spear", chance = 45000, maxCount = 2},
	{id = "venison", chance = 21000},
	{id = "meat", chance = 18000},
	{id = "piercing bolt", chance = 16000, maxCount = 5},
	{id = "elven hoof", chance = 12000},
	{id = "hunting spear", chance = 8000, maxCount = 2},
	{id = "small emerald", chance = 3600},
	{id = "crossbow", chance = 2350},
	{id = "raspberry", chance = 670, maxCount = 3},
	{id = "bullseye potion", chance = 340},
	{id = "terra rod", chance = 340}
}

mType:register(monster)
