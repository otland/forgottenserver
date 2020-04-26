local mType = Game.createMonsterType("morguthis")
local monster = {}

monster.name = "Morguthis"
monster.description = "Morguthis"
monster.experience = 3000
monster.outfit = {
	lookType = 90,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 4800
monster.maxHealth = 4800
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 6025
monster.speed = 320
monster.summonCost = 0
monster.maxSummons = 3

monster.changeTarget = {
	interval = 5000,
	chance = 8
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
	{text = "Vengeance!", yell = false},
	{text = "You will make a fine trophy.", yell = false},
	{text = "Come and fight me, cowards!", yell = false},
	{text = "I am the supreme warrior!", yell = false},
	{text = "Let me hear the music of battle.", yell = false},
	{text = "Anotherone to bite the dust!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 20},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 20},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 20},
	{type = COMBAT_HOLYDAMAGE , percent = -20},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -1000, effect = CONST_ME_DRAWBLOOD, condition = {type = CONDITION_POISON, startDamage = 65, interval = 4000}},
	{name ="combat", interval = 3000, chance = 7, type = COMBAT_LIFEDRAIN, minDamage = -55, maxDamage = -550, range = 1},
	{name ="speed", interval = 1000, chance = 25, speed = {min = -650, max = -650}, duration = 50000, range = 7, effect = CONST_ME_MAGIC_RED},
	{name ="combat", interval = 1000, chance = 20, type = COMBAT_PHYSICALDAMAGE, minDamage = -40, maxDamage = -400, radius = 3, target = false, effect = CONST_ME_HITAREA},
	{name ="combat", interval = 3000, chance = 7, type = COMBAT_PHYSICALDAMAGE, minDamage = -50, maxDamage = -500, radius = 3, target = false, effect = CONST_ME_MORTAREA}
}

monster.defenses = {
	defense = 75,
	armor = 75,
	{name ="combat", interval = 1000, chance = 13, type = COMBAT_HEALING, minDamage = 200, maxDamage = 300, effect = CONST_ME_MAGIC_BLUE},
	{name ="speed", interval = 1000, chance = 7, speed = {min = 600, max = 600}, duration = 5000, effect = CONST_ME_MAGIC_RED},
	{name ="invisible", interval = 2000, chance = 10, duration = 4000, effect = CONST_ME_MAGIC_RED}
}

monster.summons = {
	{name = "Hero", chance = 100, interval = 2000}
}

monster.loot = {
	{id = "sword hilt", chance = 100000},
	{id = "gold coin", chance = 30000, maxCount = 100},
	{id = "gold coin", chance = 30000, maxCount = 100},
	{id = "gold coin", chance = 30000, maxCount = 21},
	{id = "black pearl", chance = 10000},
	{id = "assassin star", chance = 9700, maxCount = 3},
	{id = "great health potion", chance = 9500},
	{id = "knight axe", chance = 6000},
	{id = "stone skin amulet", chance = 5000},
	{id = "mini mummy", chance = 140},
	{id = "ravager's axe", chance = 150},
	{id = "steel boots", chance = 150},
	{id = 2136, chance = 140}
}

mType:register(monster)
