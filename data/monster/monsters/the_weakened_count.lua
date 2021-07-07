local mType = Game.createMonsterType("the weakened count")
local monster = {}

monster.name = "The Weakened Count"
monster.description = "The Weakened Count"
monster.experience = 450
monster.outfit = {
	lookType = 68,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 740
monster.maxHealth = 740
monster.runHealth = 0
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
	boss = true,
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
	{text = "1... 2... 2... Uh, can't concentrate.", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 10},
	{type = COMBAT_ENERGYDAMAGE, percent = 0},
	{type = COMBAT_EARTHDAMAGE, percent = 100},
	{type = COMBAT_FIREDAMAGE, percent = -10},
	{type = COMBAT_LIFEDRAIN, percent = 100},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = -10},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -190, effect = CONST_ME_DRAWBLOOD},
	{name ="combat", interval = 2000, chance = 20, type = COMBAT_LIFEDRAIN, minDamage = 0, maxDamage = -95, range = 1, effect = CONST_ME_SMALLCLOUDS},
	{name ="speed", interval = 2000, chance = 15, speed = {min = -400, max = -400}, duration = 60000, range = 1, effect = CONST_ME_MAGIC_RED}
}

monster.defenses = {
	defense = 30,
	armor = 30,
	{name ="speed", interval = 2000, chance = 15, speed = {min = 300, max = 300}, duration = 3000, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 2000, chance = 15, type = COMBAT_HEALING, minDamage = 15, maxDamage = 25}
}

monster.loot = {
	{id = "gold coin", chance = 100000, maxCount = 92},
	{id = "the ring of the count", chance = 100000},
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
