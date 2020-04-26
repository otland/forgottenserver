local mType = Game.createMonsterType("demon parrot")
local monster = {}

monster.name = "Demon Parrot"
monster.description = "a demon parrot"
monster.experience = 225
monster.outfit = {
	lookType = 217,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 360
monster.maxHealth = 360
monster.runHealth = 0
monster.race = "blood"
monster.corpse = 6056
monster.speed = 300
monster.summonCost = 250
monster.maxSummons = 0

monster.changeTarget = {
	interval = 5000,
	chance = 20
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
	{text = "ISHH THAT THE BESHHT YOU HAVE TO OFFERRR?", yell = false},
	{text = "YOU ARRRRRE DOOMED!", yell = false},
	{text = "I SHHMELL FEEAARRR!", yell = false},
	{text = "MY SHHEED IS FEARRR AND MY HARRRVEST ISHH YOURRR SHHOUL!", yell = false},
	{text = "Your shhoooul will be mineee!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 0},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 0},
	{type = COMBAT_DEATHDAMAGE , percent = 0}
}

monster.attacks = {
	{name ="melee", interval = 1200, chance = 100, minDamage = 0, maxDamage = -100, effect = CONST_ME_DRAWBLOOD},
	{name ="drunk", interval = 1000, chance = 30, drunkenness = 25, duration = 10000, length = 5, spread = 0, effect = CONST_ME_SOUND_RED},
	{name ="combat", interval = 1000, chance = 30, type = COMBAT_LIFEDRAIN, minDamage = -25, maxDamage = -45, range = 5, ShootEffect = CONST_ANI_SUDDENDEATH},
	{name ="combat", interval = 1000, chance = 30, type = COMBAT_LIFEDRAIN, minDamage = -15, maxDamage = -45, range = 1}
}

monster.defenses = {
	defense = 35,
	armor = 30
}

monster.loot = {
	{id = "gold coin", chance = 50000, maxCount = 99}
}

mType:register(monster)
