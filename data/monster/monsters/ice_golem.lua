local mType = Game.createMonsterType("ice golem")
local monster = {}

monster.name = "Ice Golem"
monster.description = "an ice golem"
monster.experience = 295
monster.outfit = {
	lookType = 261,
	lookHead = 0,
	lookBody = 0,
	lookLegs = 0,
	lookFeet = 0,
	lookAddons = 0,
	lookMount = 0
}

monster.health = 385
monster.maxHealth = 385
monster.runHealth = 0
monster.race = "undead"
monster.corpse = 7282
monster.speed = 195
monster.summonCost = 0
monster.maxSummons = 0

monster.changeTarget = {
	interval = 2000,
	chance = 5
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
	staticAttackChance = 50,
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
	{text = "Chrrr.", yell = false},
	{text = "Crrrrk.", yell = false},
	{text = "Gnarr.", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = true},
	{type = "outfit", condition = false},
	{type = "invisible", condition = true},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 25},
	{type = COMBAT_ENERGYDAMAGE, percent = -20},
	{type = COMBAT_EARTHDAMAGE, percent = 0},
	{type = COMBAT_FIREDAMAGE, percent = 100},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 100},
	{type = COMBAT_HOLYDAMAGE , percent = 100},
	{type = COMBAT_DEATHDAMAGE , percent = 100}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -220, effect = CONST_ME_DRAWBLOOD},
	{name ="speed", interval = 1000, chance = 13, speed = {min = -800, max = -800}, duration = 20000, length = 8, spread = 0, effect = CONST_ME_ENERGYHIT},
	{name ="combat", interval = 1000, chance = 15, type = COMBAT_ICEDAMAGE, minDamage = -50, maxDamage = -85, range = 7, ShootEffect = CONST_ANI_SMALLICE, effect = CONST_ME_ICEATTACK},
	{name ="ice golem skill reducer", interval = 2000, chance = 10}
}

monster.defenses = {
	defense = 26,
	armor = 25
}

monster.loot = {
	{id = "black pearl", chance = 1612},
	{id = "small diamond", chance = 66},
	{id = "small sapphire", chance = 578},
	{id = "gold coin", chance = 50000, maxCount = 100},
	{id = "gold coin", chance = 50000, maxCount = 3},
	{id = "ice rapier", chance = 400},
	{id = "strange helmet", chance = 444},
	{id = "shard", chance = 266},
	{id = 7441, chance = 5000},
	{id = "crystal sword", chance = 177},
	{id = "strong health potion", chance = 444},
	{id = "glacier mask", chance = 111},
	{id = "frosty heart", chance = 11111}
}

mType:register(monster)
