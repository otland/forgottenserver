local mType = Game.createMonsterType("the hag")
local monster = {}

monster.name = "The Hag"
monster.description = "The Hag"
monster.experience = 510
monster.outfit = {
	lookType = 264,
	lookHead = 19,
	lookBody = 1,
	lookLegs = 40,
	lookFeet = 2,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 935
monster.maxHealth = 935
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 7349
monster.speed = 205
monster.summonCost = 0
monster.maxSummons = 2

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
	targetDistance = 5,
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
	{text = "If you think I am to old to fight then you're wrong!", yell = false},
	{text = "I've forgotten more things then you have ever learned!", yell = false},
	{text = "Let me teach you a few things youngster!", yell = false},
	{text = "I'll teach you respect for the old!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = true},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 10},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 30},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -100, effect = CONST_ME_DRAWBLOOD},
	{name ="drunk", interval = 3000, chance = 35, drunkenness = 25, duration = 15000, range = 5, radius = 1, target = true},
	{name ="speed", interval = 4000, chance = 55, speed = {min = -400, max = -400}, duration = 12000, range = 5, radius = 1, target = true, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 25,
	armor = 24,
	{name ="combat", interval = 2000, chance = 35, type = COMBAT_HEALING, minDamage = 15, maxDamage = 75, effect = CONST_ME_MAGIC_BLUE},
	{name ="invisible", interval = 3000, chance = 50, duration = 8000, effect = CONST_ME_MAGIC_BLUE}
}

monster.summons = {
	{name = "Ghost", chance = 26, interval = 2000},
	{name = "Crypt Shambler", chance = 26, interval = 2000}
}

monster.loot = {
}

mType:register(monster)
