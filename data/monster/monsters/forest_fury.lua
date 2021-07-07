local mType = Game.createMonsterType("forest fury")
local monster = {}

monster.name = "Forest Fury"
monster.description = "a forest fury"
monster.experience = 235
monster.outfit = {
	lookType = 569,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 480
monster.maxHealth = 480
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 21359
monster.speed = 210
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
	illusionable = false,
	canPushItems = true,
	canPushCreatures = true,
	staticAttackChance = 80,
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
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -125, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 1500, chance = 40, type = COMBAT_PHYSICALDAMAGE, minDamage = 0, maxDamage = -100, range = 7, radius = 4, target = true, ShootEffect = CONST_ANI_HUNTINGSPEAR, effect = CONST_ME_MAGIC_GREEN},
	{name ="forest fury skill reducer", interval = 2000, chance = 20, range = 5}
}

monster.defenses = {
	defense = 20,
	armor = 20
}

monster.loot = {
	{id = "gold coin", chance = 87250, maxCount = 53},
	{id = "crossbow", chance = 2750},
	{id = "bolt", chance = 48270, maxCount = 15},
	{id = "meat", chance = 24930},
	{id = "piercing bolt", chance = 14900, maxCount = 5},
	{id = "elvish bow", chance = 90},
	{id = "small topaz", chance = 470},
	{id = "elven hoof", chance = 7030},
	{id = "venison", chance = 13210}
}

mType:register(monster)
