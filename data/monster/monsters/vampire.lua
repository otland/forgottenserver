local mType = Game.createMonsterType("vampire")
local monster = {}

monster.name = "Vampire"
monster.description = "a vampire"
monster.experience = 305
monster.outfit = {
	lookType = 68,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 475
monster.maxHealth = 475
monster.runHealth = 30
monster.race = "undead"
monster.corpse = 6006
monster.speed = 238
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
	{text = "BLOOD!", yell = false},
	{text = "Let me kiss your neck", yell = false},
	{text = "I smell warm blood!", yell = false},
	{text = "I call you, my bats! Come!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 25},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 100},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = -25},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -150, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_LIFEDRAIN, minDamage = -50, maxDamage = -200, range = 1, effect = CONST_ME_SMALLCLOUDS},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -400, max = -400}, duration = 60000, range = 1, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 30,
	armor = 30,
	{name ="outfit", interval = 4000, chance = 10, monster = "bat", duration = 5000, effect = CONST_ME_GROUNDSHAKER},
	{name ="speed", interval = 2000, chance = 15, speed = {min = 300, max = 300}, duration = 3000, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 15, maxDamage = 25}
}

monster.loot = {
	{id = "gold coin", chance = 90230, maxCount = 60},
	{id = "vampire teeth", chance = 7600},
	{id = "blood preservation", chance = 5100},
	{id = "grave flower", chance = 1910},
	{id = "black pearl", chance = 1800},
	{id = "strong health potion", chance = 1500},
	{id = "katana", chance = 1560},
	{id = "spike sword", chance = 1000},
	{id = 2229, chance = 1000},
	{id = "ice rapier", chance = 470},
	{id = "strange helmet", chance = 420},
	{id = "bronze amulet", chance = 230},
	{id = "emerald bangle", chance = 210},
	{id = "vampire shield", chance = 200}
}

mType:register(monster)
