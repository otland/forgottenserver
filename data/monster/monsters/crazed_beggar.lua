local mType = Game.createMonsterType("crazed beggar")
local monster = {}

monster.name = "Crazed Beggar"
monster.description = "a crazed beggar"
monster.experience = 35
monster.outfit = {
	lookType = 153,
	lookHead = 21,
	lookBody = 40,
	lookLegs = 97,
	lookFeet = 19,
	lookAddons = 3,
	lookMount = 0
}

monster.health = 100
monster.maxHealth = 100
monster.runHealth = 10
monster.race = "blood"
monster.corpse = 20351
monster.speed = 200
monster.summonCost = 300
monster.maxSummons = 0

monster.changeTarget = {
	interval = 4000,
	chance = 0
}

monster.flags = {
	summonable = true,
	attackable = true,
	hostile = true,
	convinceable = true,
	pushable = true,
	boss = false,
	illusionable = true,
	canPushItems = false,
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
	{text = "Hehehe!", yell = false},
	{text = "Raahhh!", yell = false},
	{text = "You are one of THEM! Die!", yell = false},
	{text = "Wanna buy roses??", yell = false},
	{text = "They're coming!", yell = false},
	{text = "Make it stop!", yell = false}
}

monster.immunities = {
	{type = "paralyze", condition = false},
	{type = "outfit", condition = false},
	{type = "invisible", condition = false},
	{type = "bleed", condition = false}
}

monster.elements = {
	{type = COMBAT_PHYSICALDAMAGE, percent = 5},
	{type = COMBAT_ENERGYDAMAGE, percent = 20},
	{type = COMBAT_EARTHDAMAGE, percent = -10},
	{type = COMBAT_FIREDAMAGE, percent = 0},
	{type = COMBAT_LIFEDRAIN, percent = 0},
	{type = COMBAT_MANADRAIN, percent = 0},
	{type = COMBAT_DROWNDAMAGE, percent = 0},
	{type = COMBAT_ICEDAMAGE, percent = 0},
	{type = COMBAT_HOLYDAMAGE , percent = 10},
	{type = COMBAT_DEATHDAMAGE , percent = -10}
}

monster.attacks = {
	{name ="melee", interval = 2000, chance = 100, minDamage = 0, maxDamage = -25, effect = CONST_ME_DRAWBLOOD}
}

monster.defenses = {
	defense = 15,
	armor = 15
}

monster.loot = {
	{id = "small blue pillow", chance = 420},
	{id = 2072, chance = 360},
	{id = "gold coin", chance = 99000, maxCount = 9},
	{id = "dwarven ring", chance = 120},
	{id = 2237, chance = 55000},
	{id = "wooden hammer", chance = 6500},
	{id = "wooden spoon", chance = 9750},
	{id = 2570, chance = 5650},
	{id = "meat", chance = 9500},
	{id = "roll", chance = 22500},
	{id = "red rose", chance = 4700},
	{id = "sling herb", chance = 420},
	{id = "rum flask", chance = 420},
	{id = 6092, chance = 300},
	{id = 9808, chance = 80}
}

mType:register(monster)
